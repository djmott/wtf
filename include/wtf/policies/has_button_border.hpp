
#pragma once

namespace wtf{
  namespace policy{

    template <typename _SuperT, typename _ImplT> struct has_button_border :  _SuperT{

      has_button_border() : _SuperT(){
      }
    protected:
      has_button_border(window<void> * pParent) : _SuperT(pParent){}

      virtual void wm_create() override{ border_style(border_styles::raised); };
      virtual void wm_mouse_move(const policy::mouse_event& m) override{
        if (!_Down || policy::mouse_event::buttons::left != m.button) return;
        if (!rect<coord_frame::client>::get(*this).is_in(m.position)){
          ::ReleaseCapture();
          _Down = false;
          border_style(border_styles::raised);
          refresh();
        }
      };
      virtual void wm_mouse_down(const policy::mouse_event& m) override{
        if (policy::mouse_event::buttons::left != m.button) return;
        _Down = true;
        border_style(border_styles::lowered);
        ::SetCapture(*this);
        refresh();
      };
      virtual void wm_mouse_up(const policy::mouse_event& m) override{
        if (!_Down || policy::mouse_event::buttons::left != m.button) return;
        _Down = false;
        border_style(border_styles::raised);
        refresh();
        ::ReleaseCapture();
      };

    private:
      bool _Down = false;
    };
  }
}