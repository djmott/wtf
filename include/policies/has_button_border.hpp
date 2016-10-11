
#pragma once

namespace wtf{
  namespace policy{
    template <typename _SuperT, typename _ImplT> struct has_button_border : _SuperT{
      has_button_border(const has_button_border&) = delete;
      has_button_border &operator=(const has_button_border &) = delete;
      has_button_border(has_button_border&&) = delete;
      has_button_border &operator=(has_button_border&&) = delete;
      virtual ~has_button_border() = default;
      has_button_border() : _SuperT(){
        border_style(border_styles::raised);
      }

      virtual void MouseMoveEvent(const policy::mouse_event& m) override{
        _SuperT::MouseMoveEvent(m);
        if (!_Down || policy::mouse_event::buttons::left != m.button) return;
        if (!rect::client_coord::get(*this).is_in(m.position)){
          ::ReleaseCapture();
          _Down = false;
          border_style(border_styles::raised);
          refresh();
        }
      }
      virtual void MouseDownEvent(const policy::mouse_event& m) override{
        _SuperT::MouseDownEvent(m);
        if (policy::mouse_event::buttons::left != m.button) return;
        _Down = true;
        border_style(border_styles::lowered);
        ::SetCapture(*this);
        refresh();
      }
      virtual void MouseUpEvent(const policy::mouse_event& m) override{
        _SuperT::MouseUpEvent(m);
        if (!_Down || policy::mouse_event::buttons::left != m.button) return;
        _Down = false;
        border_style(border_styles::raised);
        refresh();
        ::ReleaseCapture();
      }

    private:
      bool _Down = false;
    };
  }
}