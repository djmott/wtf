#pragma once
namespace wtf{
  struct tree : window<tree, policy::has_border, policy::has_click, policy::has_text,
    policy::has_paint, policy::has_size, policy::has_click, policy::has_mouse_wheel, policy::has_font>{

    explicit tree(HWND hParent) : window(hParent), _vscroll(*this), _hscroll(*this), _background_brush(brush::system_brush(system_colors::window))
    {
      _vscroll.orientation(scroll_bar::orientations::vertical);
      _hscroll.orientation(scroll_bar::orientations::horizontal);
      auto_draw_text(false);
    }

    bool full_row_select() const{ return _full_row_select; }
    void full_row_select(bool newval){ _full_row_select = newval; }

    enum class select_modes{
      single,
      multi,
      extended,
    };

    select_modes select_mode() const{ return _select_mode; }
    void select_mode(select_modes newval){ _select_mode = newval; }

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

      bool selected() const{ return _selected; }
      void selected(bool newval){ _selected = newval; }

      pointer parent(){ return _parent.lock(); }

      const vector& children() const{ return _children; }

      pointer add_child(const tstring& Text){
        _children.push_back(pointer(new node(shared_from_this(), Text)));
        return _children.back();
      }
      void add_child(pointer oChild){
        oChild->_parent = shared_from_this();
        _children.push_back(oChild);
      }

      pointer next_sibbling() {
        auto oParent = parent();
        if (!oParent) return oParent;
        for (int i = 0; i < oParent->children().size() - 1; ++i){
          if (this == oParent->children()[i].get()) return oParent->children()[1 + i];
        }
        return pointer();
      }
      pointer prev_sibbling() {
        auto oParent = parent();
        if (!oParent) return oParent;
        for (size_t i = oParent->children().size(); i >= 1; --i){
          if (this == oParent->children()[i].get()) return oParent->children()[i - 1];
        }
        return pointer();
      }


    private:
      bool _selected = false;
      tstring _text;
      bool _expanded = false;
      vector _children;
      weak_ptr _parent;
      expander_display_policies _expander_display_policy = expander_display_policies::with_children;
    };

    node::pointer add_node(const tstring& name){
      auto oRet = std::make_shared<node>(node::pointer(), name);
      if (!_top) _top = oRet;
      _nodes.push_back(oRet);
      return oRet;
    }

  protected:
    static const int scroll_width = 15;
    static const int left_margin = 2;
    static const int right_margin = 2;
    static const int top_margin = 2;
    static const int bottom_margin = 2;
    static const int node_indent = 15;
    static const int expander_width = 10;
    
    select_modes _select_mode = select_modes::multi;
    bool _full_row_select = true;
    node::vector _nodes;
    node::pointer _top;
    rect::client_coord::vector _ItemRects;
    rect::client_coord::vector _ExpanderRects;
    rect::client_coord::vector _RowRects;
    node::vector _DisplayedNodes;
    brush _background_brush;

    virtual void ClickEvent(const policy::mouse_event& m) override { 
      if (policy::mouse_event::buttons::left != m.button) return;
      for (size_t i = 0; i < _RowRects.size(); ++i){
        if (!_RowRects[i].is_in(m.position)) continue;
        _DisplayedNodes[i]->selected(true);
        refresh();
        break;
      }
    }

    static int get_item_depth(const node::pointer& oNode){
      if (!oNode) return 0;
      return 1 + get_item_depth(oNode->parent());
    }

    virtual void ResizedEvent(wm_size_flags, const point::client_coords& p) override{
      _vscroll.move(p.x - scroll_width - right_margin, top_margin, scroll_width, p.y - top_margin - bottom_margin - scroll_width);
      _hscroll.move(left_margin, p.y - bottom_margin - scroll_width, p.x - scroll_width - right_margin - left_margin, scroll_width);
    };


    bool print_nodes(const device_context& dc, const node::pointer& oNode, int & iTop, int iLeft, const rect::client_coord& oMaxExtents){
      if (iTop > oMaxExtents.bottom) return false;

      auto blackPen = pen::create(pen::style::solid, 1, rgb(255, 255, 255));
      auto blackBrush = brush::solid_brush(rgb(255, 255, 255));
      auto whiteBrush = brush::solid_brush(rgb(0, 0, 0));

      auto oTextSize = dc.get_text_extent(oNode->text());

      _DisplayedNodes.push_back(oNode);

      auto iTmpLeft = iLeft;
      bool bPrintExpander = false;
      bool bExpanded = false;

      if (node::expander_display_policies::always == oNode->expander_display_policy()){
        bPrintExpander = true;
        bExpanded = oNode->expanded();
      } else if (node::expander_display_policies::never == oNode->expander_display_policy()){
        bPrintExpander = false;
        bExpanded = false;
      } else{
        bPrintExpander = oNode->children().size() ? true : false;
        bExpanded = oNode->expanded();
      }

      if (bPrintExpander){
        _ExpanderRects.emplace_back(iTmpLeft, iTop, iTmpLeft + expander_width, iTop + oTextSize.cy);
        iTmpLeft += expander_width + 5;
      } else{
        _ExpanderRects.emplace_back(-1, -1, -1, -1);
      }
      auto & oExpanderRect = _ExpanderRects.back();

      if (bPrintExpander){
        point::client_coords::vector arrow(3);
        if (bExpanded){
          arrow[0].x = oExpanderRect.left; arrow[0].y = oExpanderRect.top;
          arrow[1].x = oExpanderRect.right; arrow[1].y = oExpanderRect.top;
          arrow[2].x = oExpanderRect.right + (expander_width/2); arrow[2].y = oExpanderRect.bottom;
          dc.fill(arrow, blackPen, blackBrush);
        } else{
          arrow[0].x = oExpanderRect.left; arrow[0].y = oExpanderRect.top;
          arrow[1].x = oExpanderRect.right; arrow[1].y = oExpanderRect.top + (expander_width/2);
          arrow[2].x = oExpanderRect.left; arrow[2].y = oExpanderRect.bottom;
          dc.fill(arrow, blackPen, whiteBrush);
        }
      }

      _ItemRects.emplace_back(iTmpLeft, iTop, iTmpLeft + oTextSize.cx , iTop + oTextSize.cy);
      text(oNode->text());
      draw_text(dc, _ItemRects.back());

      rect::client_coord oRowRect(oMaxExtents.left, iTop, oMaxExtents.right, oTextSize.cy);
      _RowRects.push_back(oRowRect);

      iTop += oTextSize.cy;

      if (oNode->selected()){
        if (full_row_select()){
          dc.invert(oRowRect);
        } else{
          dc.invert(_ItemRects.back());
        }
      }

      if (bExpanded){
        for (auto & oChild : oNode->children()){
          if (!print_nodes(dc, oChild, iTop, iTmpLeft + node_indent, oMaxExtents)) break;
        }
      }

      return true;
    }

    virtual const brush &background_brush() const{ return _background_brush; }


    virtual void PaintEvent(const device_context& dc, const paint_struct& ps) override{
      if (!_nodes.size()) return;
      ApplyFontEvent(dc);
      auto client = ps.client();
      client.bottom -= scroll_width;
      client.right -= scroll_width;
      client.left += left_margin;
      client.top += top_margin;
      _ItemRects.clear();
      _ExpanderRects.clear();
      _DisplayedNodes.clear();
      _RowRects.clear();
      int iTop = top_margin;
      for (auto & oNode : _nodes){
        if (!print_nodes(dc, oNode, iTop, left_margin, client)) break;
      }

    }

    void ScrollUp(){
      if (!_top) return;
      auto oPrev = _top->prev_sibbling();
      auto oParent = _top->parent();
      if (!oParent && !oPrev) return;
      if (oPrev){
        _top = oPrev;
      } else if (oParent){
        _top = oParent;
      }
      refresh();
    }

    void ScrollDown(){
      if (_top->expanded() && _top->children().size()){
        _top = _top->children()[0];
      } else{
        auto oNext = _top->next_sibbling();
        if (oNext) _top = oNext;
      }
      refresh();
    }
    void ScrollLeft(){}
    void ScrollRight(){}

    struct vscroll : scroll_bar{
      vscroll(tree& parent) : scroll_bar(parent), _parent(parent){}
      virtual void IncrementEvent() override{ _parent.ScrollUp(); }
      virtual void DecrementEvent() override{ _parent.ScrollDown(); }
      tree& _parent;
    }_vscroll;

    struct hscroll : scroll_bar{
      hscroll(tree& parent) : scroll_bar(parent), _parent(parent){}
      virtual void IncrementEvent() override{}
      virtual void DecrementEvent() override{}
      tree& _parent;
    }_hscroll;
  };
}