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

      virtual void MouseMoveEvent(event_vkeys k, const point::client_coords& p) override{
        _SuperT::MouseMoveEvent(k, p);
        if (!_Down) return;
        if (!rect::client_coord::get(*this).is_in(p)){
          ::ReleaseCapture();
          _Down = false;
          border_style(border_styles::raised);
          refresh();
        }
      }
      virtual void MouseLButtonDownEvent(event_vkeys k, const point::client_coords&p) override{
        _SuperT::MouseLButtonDownEvent(k, p);
        _Down = true;
        border_style(border_styles::lowered);
        ::SetCapture(*this);
        refresh();
      }
      virtual void MouseLButtonUpEvent(event_vkeys k, const point::client_coords&p) override{
        _SuperT::MouseLButtonUpEvent(k, p);
        if (!_Down) return;
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