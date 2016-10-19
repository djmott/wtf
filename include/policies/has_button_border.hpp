
#pragma once

namespace wtf{
  namespace policy{

    template <typename _SuperT, typename _ImplT> struct has_button_border :  _SuperT{

      has_button_border() : _SuperT(){
        OnCreate += [this](){ border_style(border_styles::raised); };
        OnMouseMove += [this](const policy::mouse_event& m){
          if (!_Down || policy::mouse_event::buttons::left != m.button) return;
          if (!rect::client_coord::get(*this).is_in(m.position)){
            ::ReleaseCapture();
            _Down = false;
            border_style(border_styles::raised);
            refresh();
          }
        };
        OnMouseDown += [this](const policy::mouse_event& m){
          if (policy::mouse_event::buttons::left != m.button) return;
          _Down = true;
          border_style(border_styles::lowered);
          ::SetCapture(*this);
          refresh();
        };
        OnMouseUp += [this](const policy::mouse_event& m){
          if (!_Down || policy::mouse_event::buttons::left != m.button) return;
          _Down = false;
          border_style(border_styles::raised);
          refresh();
          ::ReleaseCapture();
        };
      }
    protected:
      has_button_border(iwindow * pParent) : _SuperT(pParent){}
    private:
      bool _Down = false;
    };
  }
}