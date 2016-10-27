#pragma once


namespace wtf{

  template <typename _ImplT, policy..._Policies>
  class window<_ImplT, policy::isa_listbox, _Policies...> 
    : public window_impl<_ImplT, _Policies..., policy::isa_label, policy::wm_mouse_wheel>
  {
    using __listbox_t = window<_ImplT, policy::isa_listbox, _Policies...>;
    using __super_t = window_impl<_ImplT, _Policies..., policy::isa_label, policy::wm_mouse_wheel>;
    template <typename, policy ... > friend class window_impl;
  public:


    explicit window(iwindow * pParent) :
      __super_t(pParent),
      _SelectedItems(1, 0),
      _vscroll(this)
    {}

    virtual void on_wm_create() override{
      border_style(border_styles::raised);
      auto_draw_text(false);
      __super_t::on_wm_create();
    };

    virtual void on_wm_size(const point<coord_frame::client>& p) override{
      _vscroll.move(p.x - scroll_width - right_margin, top_margin, scroll_width, p.y - top_margin - bottom_margin);
      __super_t::on_wm_size(p);
    };
    
    virtual void on_wm_paint(const device_context& dc, const paint_struct& ps) override{
      if (!_Items.size()) return __super_t::on_wm_paint(dc, ps);
      auto client = ps.client();
      auto oTextSize = dc.get_text_extent(_Items[0]);
      _ItemRects.clear();
      int listWidth = client.right - right_margin - scroll_width - left_margin;
      for (int i = top_margin; i < client.bottom; i += oTextSize.cy){
        _ItemRects.push_back(rect<coord_frame::client>(left_margin, i, listWidth, std::min(i + oTextSize.cy, client.bottom - bottom_margin)));
      }

      for (size_t i = 0; i < _ItemRects.size() && (_TopIndex + i) < _Items.size(); ++i){
        for (size_t x = 0; x < _SelectedItems.size(); x++){
          if (_SelectedItems[x] == (_TopIndex + static_cast<int>(i))){
            dc.fill(_ItemRects[i], brush::system_brush(system_colors::highlight));
            break;
          }
        }
        text(_Items[i + _TopIndex]);
        draw_text(dc, _ItemRects[i]);
      }
      __super_t::on_wm_paint(dc, ps);
    };

    virtual void on_wm_mouse_wheel(int16_t delta, const mouse_msg_param& param) override{
      if (delta > 0) _vscroll.StepDecEvent();
      else _vscroll.StepIncEvent();
      __super_t::on_wm_mouse_wheel(delta, param);
    };

    virtual void on_wm_click(const mouse_msg_param& m) override{
      if (mouse_msg_param::buttons::left != m.button) return;
      if (selection_modes::single == _selection_mode){
        _SelectedItems.clear();
      }
      for (size_t i = 0; i < _ItemRects.size() && (_TopIndex + i) < _Items.size(); ++i){
        if (!_ItemRects[i].is_in(m.position)) continue;
        _SelectedItems.push_back(_TopIndex + static_cast<int>(i));
      }
      invalidate();
      __super_t::on_wm_click(m);
    };

    static const int scroll_width = 15;
    static const int left_margin = 2;
    static const int right_margin = 2;
    static const int top_margin = 2;
    static const int bottom_margin = 2;

    enum class selection_modes{
      single,
      multiple,
    };

    selection_modes selection_mode() const{ return _selection_mode; }
    void selection_mode(selection_modes newval){ _selection_mode = newval; }

    void add_item(const tstring& newval){
      _Items.push_back(newval);
    }
  protected:
    friend struct vscroll;
    void invalidate(){ __super_t::invalidate(); }

    virtual void handle_msg(window_message& msg) override{}

    class vscroll : public window<vscroll, policy::isa_scrollbar> {
      using __super_t = window<vscroll, policy::isa_scrollbar>;
      template <typename, policy ... > friend class window_impl;
    public:

      virtual void handle_msg(window_message& msg) override{}
      vscroll(__listbox_t * pParent) : __super_t(pParent), _Parent(pParent){}

      virtual void on_wm_create() override{ 
        orientation(orientations::vertical); 
        __super_t::on_wm_create();
      };

      virtual void StepIncEvent(){
        if ((_Parent->_TopIndex + _Parent->_ItemRects.size()) < _Parent->_Items.size()) _Parent->_TopIndex++;
        _Parent->invalidate();
      }

      virtual void StepDecEvent(){
        if (!_Parent->_TopIndex) return;
        --_Parent->_TopIndex;
        _Parent->invalidate();
      }
      __listbox_t * _Parent;
    };

  private:

    UINT_PTR _MouseDownTimer;
    bool _IncrementHeldDown;
    point<coord_frame::client> _MouseDownAt;
    int _TopIndex = 0;
    std::vector<tstring> _Items;
    rect<coord_frame::client>::vector _ItemRects;
    std::vector<int> _SelectedItems;
    vscroll _vscroll;
    selection_modes _selection_mode = selection_modes::single;
    brush _background_brush = brush::system_brush(system_colors::window);
  };

  struct listbox : window<listbox, policy::isa_listbox>{
    explicit listbox(iwindow * pParent) : window(pParent){}
  };

}
