#pragma once
namespace wtf{

  struct listbox : wtf::window<listbox, policy::has_border, policy::has_click, policy::has_text, 
    policy::has_move, policy::has_background, policy::has_size, policy::has_font, messages::wm_paint, 
    messages::wm_mouse_wheel, messages::wm_create, messages::wm_size, messages::wm_erasebkgnd,
    messages::wm_mouse_down, messages::wm_mouse_up, messages::wm_nccalcsize, messages::wm_ncpaint>
  {

    using mouse_msg_param = messages::mouse_msg_param;


    explicit listbox(window<void,void> * pParent) :
      window(pParent),
      _TopIndex(0),
      _SelectedItems(1, 0),
      _vscroll(this),
      _selection_mode(selection_modes::single),
      _background_brush(brush::system_brush(system_colors::window))
    {}

    virtual LRESULT on_wm_create(bool& bHandled) override{
      border_style(border_styles::raised);
      auto_draw_text(false);
      return window::on_wm_create(bHandled);
    };

    virtual LRESULT on_wm_size(const point<coord_frame::client>& p, bool& bHandled) override{
      _vscroll.move(p.x - scroll_width - right_margin, top_margin, scroll_width, p.y - top_margin - bottom_margin);
      return window::on_wm_size(p, bHandled);
    };
    
    virtual LRESULT on_wm_paint(const device_context& dc, const paint_struct& ps, bool& bHandled) override{
      if (!_Items.size()) return window::on_wm_paint(dc, ps, bHandled);
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
      return window::on_wm_paint(dc, ps, bHandled);
    };

    virtual LRESULT on_wm_mouse_wheel(int16_t delta, const mouse_msg_param& param, bool& bHandled) override{
      if (delta > 0) _vscroll.StepDecEvent();
      else _vscroll.StepIncEvent();
      return window::on_wm_mouse_wheel(delta, param, bHandled);
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
      window::on_wm_click(m);
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
    void invalidate(){ window::invalidate(); }
    struct vscroll : scroll_bar{


      vscroll(listbox * pParent) : scroll_bar(pParent), _Parent(pParent){}

      virtual LRESULT on_wm_create(bool& bHandled) override{ 
        orientation(orientations::vertical); 
        return scroll_bar::on_wm_create(bHandled);
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
      listbox * _Parent;
    };

  private:

    UINT_PTR _MouseDownTimer;
    bool _IncrementHeldDown;
    point<coord_frame::client> _MouseDownAt;
    int _TopIndex;
    std::vector<tstring> _Items;
    rect<coord_frame::client>::vector _ItemRects;
    std::vector<int> _SelectedItems;
    vscroll _vscroll;
    selection_modes _selection_mode;
    brush _background_brush;
  };

}
