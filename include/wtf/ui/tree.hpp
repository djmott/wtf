#pragma once

namespace wtf{
  struct tree : window<tree, policy::has_border, policy::has_click, policy::has_text,
    policy::has_create, policy::has_paint, policy::has_size, policy::has_dblclick, 
    policy::has_mouse_wheel, policy::has_font, policy::has_move>
  {

    using mouse_msg_param = messages::mouse_msg_param;

    explicit tree(window<void> * pParent)
      : window(pParent),
      _root(new node(this)),
      _black_pen(pen::create(pen::style::solid, 1, rgb(0, 0, 0))),
      _background_brush(brush::system_brush(system_colors::window)),
      _button_face_brush(brush::system_brush(system_colors::button_face)),
      _black_brush(brush::solid_brush(rgb(0, 0, 0))),
      _white_brush(brush::solid_brush(rgb(255, 255, 255))),
      _highlight_background_brush(brush::system_brush(system_colors::highlight)),
      _highlight_text_brush(brush::system_brush(system_colors::highlight_text)),
      _vscroll(this),
      _hscroll(this)
    {}


    virtual void wm_create() override{
      _vscroll.orientation(scroll_bar::orientations::vertical);
      _hscroll.orientation(scroll_bar::orientations::horizontal);
      auto_draw_text(false);
      text_vertical_alignment(text_vertical_alignments::center);
      text_horizontal_alignment(text_horizontal_alignments::left);
      full_row_select(false);
    };

    virtual void wm_click(const mouse_msg_param& m) override{
      if (mouse_msg_param::buttons::left != m.button) return;
      node::pointer oClickedNode;
      for (size_t i = 0; i < _row_rects.size(); ++i){
        if (_expander_rects[i].is_in(m.position)){
          _displayed_nodes[i]->expanded(!_displayed_nodes[i]->expanded());
          refresh();
          return;
        } else if (_item_rects[i].is_in(m.position) || (_full_row_select && _row_rects[i].is_in(m.position))){
          oClickedNode = _displayed_nodes[i];
          break;
        }
      }
      if (!oClickedNode) return;
      if (select_modes::single == _select_mode){
        bool bExists = _selected_nodes.end() != std::find_if(_selected_nodes.begin(), _selected_nodes.end(), [oClickedNode](node::pointer oNode){ return oClickedNode.get() != oNode.get(); });
        _selected_nodes.clear();
        _selected_nodes.push_back(oClickedNode);
        if (!bExists) OnNodeSelected(oClickedNode);
      } else if (select_modes::multi == _select_mode){
        bool bSelected = !oClickedNode->selected();
        oClickedNode->selected(bSelected);
        if (!bSelected) OnNodeSelected(oClickedNode);
      } else{ //extended
        if (!(m.key_state & mouse_msg_param::key_states::control)){
          _selected_nodes.clear();
        }
        bool bSelected = !oClickedNode->selected();
        oClickedNode->selected(bSelected);
        if (!bSelected) OnNodeSelected(oClickedNode);
      }
      refresh();
    };

    virtual void wm_dblclick(const mouse_msg_param& m) override{
      if (mouse_msg_param::buttons::left != m.button) return;
      for (size_t i = 0; i < _row_rects.size(); ++i){
        if (_item_rects[i].is_in(m.position)){
          _displayed_nodes[i]->expanded(!_displayed_nodes[i]->expanded());
        } else{
          continue;
        }
        refresh();
        break;
      }
    };

    virtual void wm_paint(const device_context& dc, const paint_struct& ps) override{
      if (!_root->children().size()) return;
      ApplyFontEvent(dc);
      auto client = ps.client();
      _item_rects.clear();
      _expander_rects.clear();
      _displayed_nodes.clear();
      _row_rects.clear();
      auto oTextMetrics = text_metrics::get(dc);
      for (auto oNode = _top; oNode; oNode = oNode->get_next(false)){
        if (!print_node(oNode, dc, oTextMetrics, client)) break;
      }
    };

    virtual void wm_mouse_wheel(int16_t delta, const mouse_msg_param& m) override{
      bool bUp = (delta > 0);
      for (int i = 0; i <= (delta % WHEEL_DELTA); ++i){
        if (bUp){
          ScrollPrev();
        } else{
          ScrollNext();
        }
      }
    };

    virtual void wm_size(const point<coord_frame::client>& p) override{
      _vscroll.move(p.x - scroll_width - border_width(), border_width(), scroll_width, p.y - (border_width() * 2) - scroll_width);
      _hscroll.move(border_width(), p.y - border_width() - scroll_width, p.x - scroll_width - (border_width() * 2), scroll_width);
    };

    virtual void wm_erase_background(const device_context& ctx, const rect<coord_frame::client>& client, bool& handled) override{
      ctx.fill(client, background_brush());
      rect<coord_frame::client> oTmp(client.right - scroll_width - (border_width() * 2),
                              client.bottom - scroll_width - (border_width() * 2),
                              client.right, client.bottom);
      ctx.fill(oTmp, _button_face_brush);
    };

    bool full_row_select() const{ return _full_row_select; }
    void full_row_select(bool newval){ _full_row_select = newval; }

    enum class select_modes{
      single,
      multi,
      extended,
    };

    select_modes select_mode() const{ return _select_mode; }
    void select_mode(select_modes newval){ _select_mode = newval; }


    enum class edit_modes{
      disabled,
      automatic,
    };

    edit_modes edit_mode() const{ return _edit_mode; }
    void edit_mode(edit_modes newval){ _edit_mode = newval; }


    struct node : std::enable_shared_from_this<node>{
      using pointer = std::shared_ptr<node>;
      using weak_ptr = std::weak_ptr<node>;
      using vector = std::vector<pointer>;


      node(weak_ptr Parent, const tstring& Text) : _text(Text), _children(), _parent(Parent){}
      explicit node(const tstring& Text) : node(pointer(), Text){}
      node() : node(_T("")){}

      enum class expander_display_policies{
        always,
        never,
        with_children,
      };

      expander_display_policies expander_display_policy() const{ return _expander_display_policy; }
      void expander_display_policy(expander_display_policies newval){ _expander_display_policy = newval; }

      const tstring& text() const{ return _text; }
      void text(const tstring& newval){ _text = newval; }

      bool expanded() const{ return _expanded; }
      void expanded(bool newval){ _expanded = newval; }

      bool selected() const{
        const auto & oSelectedNodes = get_tree()._selected_nodes;
        return oSelectedNodes.cend() != std::find_if(oSelectedNodes.cbegin(), oSelectedNodes.cend(),
                                                     [this](const node::pointer& oNode){ return oNode.get() == this;  });
      }
      void selected(bool newval){
        auto & oSelectedNodes = get_tree()._selected_nodes;
        if (newval){
          if (oSelectedNodes.cend() == std::find_if(oSelectedNodes.cbegin(), oSelectedNodes.cend(),
              [this](const node::pointer& oNode){ return oNode.get() == this;  })){
            oSelectedNodes.push_back(shared_from_this());
          }
        } else{
          auto oItem = std::find_if(oSelectedNodes.cbegin(), oSelectedNodes.cend(),
                                    [this](const node::pointer& oNode){ return oNode.get() == this;  });
          oSelectedNodes.erase(oItem);
        }
      }

      pointer parent() const{ return _parent.lock(); }

      const vector& children() const{ return _children; }

      pointer add_node(const tstring& Text){
        _children.push_back(pointer(new node(shared_from_this(), Text)));
        return _children.back();
      }
      void add_node(pointer oChild){
        oChild->_parent = shared_from_this();
        _children.push_back(oChild);
      }

    private:
      friend struct tree;

      node(tree * pTree) : _tree(pTree){}

      tree& get_tree(){
        if (_tree) return *_tree;
        return parent()->get_tree();
      }
      const tree& get_tree() const{
        if (_tree)  return *_tree;
        return parent()->get_tree();
      }

      pointer get_last(){
        if (!children().size() || !expanded()) return shared_from_this();
        return children().back()->get_last();
      }

      pointer get_next(bool recurse_to_children = true){
        if (recurse_to_children && children().size() && expanded()) return _children[0];
        auto oParent = parent();
        if (!oParent) return oParent;
        const auto & oSibblings = oParent->children();
        for (int i = 0; i < (static_cast<int>(oSibblings.size()) - 2); ++i){
          if (oSibblings[i].get() == this) return oSibblings[i + 1];
        }
        return oParent->get_next(false);
      }

      pointer get_prev(){
        auto oParent = parent();
        if (!oParent) return oParent;
        const auto & oSibblings = oParent->children();
        for (int i = 1; i < oSibblings.size() - 1; ++i){
          if (oSibblings[i].get() == this) return oSibblings[i - 1]->get_last();
        }
        if (oParent->parent()) return oParent;
        return pointer();
      }

      int depth(){
        auto oParent = parent();
        if (oParent) return 1 + oParent->depth();
        return 1;
      }

      tree * _tree = nullptr;
      bool _selected = false;
      tstring _text;
      bool _expanded = false;
      vector _children;
      weak_ptr _parent;
      expander_display_policies _expander_display_policy = expander_display_policies::with_children;
    };

    callback <void(node::pointer)> OnNodeSelected;

    node::pointer add_node(const tstring& Text){
      auto oNode = _root->add_node(Text);
      if (!_top) _top = oNode;
      if (!_bottom) _bottom = oNode;
      refresh();
      return oNode;
    }

    void add_node(node::pointer oNode){
      _root->add_node(oNode);
      if (!_top) _top = oNode;
      if (!_bottom) _bottom = oNode;
      refresh();
    }

    const node::vector& selected_items() const{ return _selected_nodes; }

  protected:
    static const int scroll_width = 15;

    node::pointer _root;
    node::pointer _top; //node at the top of the display window
    node::pointer _bottom; //node at the bottom of the display window
    rect<coord_frame::client>::vector _item_rects; //rect around item text
    rect<coord_frame::client>::vector _expander_rects; //rect around expander
    rect<coord_frame::client>::vector _row_rects; //rect around entire row
    node::vector _displayed_nodes; //the displayed items
    node::vector _selected_nodes;
    pen _black_pen;
    brush _background_brush;
    brush _button_face_brush;
    brush _black_brush;
    brush _white_brush;
    brush _highlight_background_brush;
    brush _highlight_text_brush;
    select_modes _select_mode = select_modes::extended;
    edit_modes _edit_mode = edit_modes::disabled;
    bool _full_row_select = true;
    bool _disable_scroll_up = false;


    static int get_item_depth(const node::pointer& oNode){
      if (!oNode) return 0;
      return 1 + get_item_depth(oNode->parent());
    }

    virtual const brush &background_brush() const{ return _background_brush; }


    bool print_node(const node::pointer& oNode, const device_context& dc, const text_metrics& oTextMetrics, rect<coord_frame::client>& oClient){
      if (oClient.top > oClient.bottom) return false;
      _bottom = oNode;
      bool PrintExpander = false;
      if (node::expander_display_policies::always == oNode->expander_display_policy()){
        PrintExpander = true;
      } else if (node::expander_display_policies::with_children == oNode->expander_display_policy()){
        PrintExpander = oNode->children().size() ? true : false;
      }
      auto iExpanderWidth = 8;
      auto iItemLeft = oNode->depth() * oTextMetrics.tmAveCharWidth * 2;
      auto iExpanderLeft = iItemLeft - iExpanderWidth - 5;
      auto iExpanderTop = (oTextMetrics.tmHeight - iExpanderWidth) / 2;
      auto oTE = dc.get_text_extent(oNode->text());
      _item_rects.emplace_back(iItemLeft, oClient.top, iItemLeft + oTE.cx, oClient.top + oTE.cy);
      _expander_rects.emplace_back(iExpanderLeft, oClient.top + iExpanderTop, iExpanderLeft + iExpanderWidth, oClient.top + iExpanderTop + iExpanderWidth);
      _row_rects.emplace_back(oClient.left, oClient.top, oClient.right, oClient.top + oTextMetrics.tmHeight);
      _displayed_nodes.push_back(oNode);

      if (oNode->selected()){
        if (_full_row_select){
          dc.fill(_row_rects.back(), _highlight_background_brush);
        } else{
          dc.fill(_item_rects.back(), _highlight_background_brush);
        }
        dc.text_color(system_rgb<system_colors::highlight_text>());
      } else{
        dc.text_color(system_rgb<system_colors::button_text>());
      }

      text(oNode->text());

      draw_text(dc, _item_rects.back());
      if (PrintExpander){
        const auto & oExpander = _expander_rects.back();
        point<coord_frame::client>::vector oArrow(3);
        if (oNode->expanded()){
          oArrow[0].x = oExpander.left; oArrow[0].y = oExpander.top;
          oArrow[1].x = oExpander.right; oArrow[1].y = oExpander.top;
          oArrow[2].x = oExpander.left + ((oExpander.right - oExpander.left) / 2); oArrow[2].y = oExpander.bottom;
          dc.fill(oArrow, _black_pen, _black_brush);
        } else{
          oArrow[0].x = oExpander.left; oArrow[0].y = oExpander.top;
          oArrow[1].x = oExpander.right; oArrow[1].y = oExpander.top + ((oExpander.bottom - oExpander.top) / 2);
          oArrow[2].x = oExpander.left; oArrow[2].y = oExpander.bottom;
          dc.fill(oArrow, _black_pen, _white_brush);
        }
      }
      oClient.top += oTextMetrics.tmHeight;
      if (oNode->children().size() && oNode->expanded()){
        for (const auto & oChild : oNode->children()){
          if (!print_node(oChild, dc, oTextMetrics, oClient)) return false;
        }
      }
      return true;
    }


    void ScrollPrev(){
      if (!_top || !_bottom) return;
      auto oTopPrev = _top->get_prev();
      auto oBottomPrev = _bottom->get_prev();
      if (!oTopPrev || !oBottomPrev) return;
      _top = oTopPrev;
      _bottom = oBottomPrev;
      refresh();
    }

    void ScrollNext(){
      if (!_top || !_bottom) return;
      auto oTopNext = _top->get_next(true);
      auto oBottomNext = _bottom->get_next(true);
      if (!oTopNext || !oBottomNext) return;
      _top = oTopNext;
      _bottom = oBottomNext;
      refresh();
    }

    void ScrollLeft(){}
    void ScrollRight(){}


    struct vscroll : scroll_bar{
      vscroll(tree * parent) : scroll_bar(parent), _parent(parent){}
      virtual void StepIncEvent() override{ _parent->ScrollNext(); }
      virtual void StepDecEvent() override{ _parent->ScrollPrev(); }
      tree * _parent;
    }_vscroll;

    struct hscroll : scroll_bar{
      hscroll(tree * parent) : scroll_bar(parent), _parent(parent){}
      virtual void StepIncEvent() override{}
      virtual void StepDecEvent() override{}
      tree * _parent;
    }_hscroll;
  };
}