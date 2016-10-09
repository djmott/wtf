#pragma once
namespace wtf{
  struct tree : window<tree, policy::has_border, policy::has_click, policy::has_text,
    policy::has_paint, policy::has_size, policy::has_mouse, policy::has_font>{

    explicit tree(HWND hParent) : window(hParent), _vscroll(*this), _hscroll(*this)
    {
    }

    bool full_row_select() const{ return _full_row_select; }
    void full_row_select(bool newval){ _full_row_select = newval; }

    struct node : std::enable_shared_from_this<node>{
      using pointer = std::shared_ptr<node>;
      using weak_ptr = std::weak_ptr<node>;
      using vector = std::vector<pointer>;


      node(weak_ptr Parent, const tstring& Text) : _text(Text), _expanded(false), _children(), _parent(Parent){}
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
      tstring _text;
      bool _expanded;
      vector _children;
      weak_ptr _parent;
      expander_display_policies _expander_display_policy = expander_display_policies::with_children;
    };


  protected:
    static const int scroll_width = 15;
    static const int left_margin = 2;
    static const int right_margin = 2;
    static const int top_margin = 2;
    static const int bottom_margin = 2;
    static const int node_indent = 20;
    static const int expander_width = 10;

    bool _full_row_select = true;
    node::vector _nodes;
    node::pointer _top;
    rect::client_coord::vector _ItemRects;
    rect::client_coord::vector _ExpanderRects;
    rect::client_coord::vector _RowRects;
    node::vector _DisplayedNodes;

    static int get_item_depth(const node::pointer& oNode){
      if (!oNode) return 0;
      return 1 + get_item_depth(oNode->parent());
    }

    virtual void ResizedEvent(wm_size_flags, const point::client_coords& p) override{
      _vscroll.move(p.x - scroll_width - right_margin, top_margin, scroll_width, p.y - top_margin - bottom_margin - scroll_width);
      _hscroll.move(left_margin, p.y - bottom_margin - scroll_width, p.x - scroll_width - right_margin - left_margin, scroll_width);
    };


    bool print_nodes(const device_context& dc, node::pointer& oNode, int & iTop, int iLeft, const rect::client_coord& oMaxExtents, const size& oFontSize){
      if (iTop > oMaxExtents.bottom) return false;
      _RowRects.push_back(rect::client_coord(left_margin, iTop, oMaxExtents.right, oFontSize.cy));
      _DisplayedNodes.push_back(oNode);
      auto iCurrLeft = iLeft;
      if (iCurrLeft < oMaxExtents.right){
        if (node::expander_display_policies::always == oNode->expander_display_policy() ||
          (node::expander_display_policies::with_children == oNode->expander_display_policy() && oNode->children().size()))
        {
          _ExpanderRects.push_back(rect::client_coord(iCurrLeft, iTop, expander_width, expander_width));
          iCurrLeft += expander_width;
            
        } else{
          _ExpanderRects.push_back(rect::client_coord(0, 0, 0, 0));
        }
      }


      if (oNode->expanded()){
        for (const auto & oChild : oNode->children()){
//          if (!print_nodes(oChild, iTop + oFontSize.cy, iLeft + node_indent, oMaxExtents, oFontSize)) return false;
        }
      }

    }

    virtual void PaintEvent(const device_context& dc, const paint_struct& ps) override{
      if (!_nodes.size()) return;
      ApplyFontEvent(dc);
      auto client = ps.client();
      size oTextSize(font().width(), font().height());
      _ItemRects.clear();
      _ExpanderRects.clear();
      _DisplayedNodes.clear();
      _RowRects.clear();
      int iTop = top_margin;
//      print_nodes(_top, iTop, get_item_depth(_top) * node_indent, client, oTextSize);
    }



    struct vscroll : scroll_bar{
      vscroll(tree& parent) : scroll_bar(parent){}
    }_vscroll;

    struct hscroll : scroll_bar{
      hscroll(tree& parent) : scroll_bar(parent){}
    }_hscroll;
  };
}