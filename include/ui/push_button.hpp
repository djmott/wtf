#pragma once

namespace wtf{

  struct push_button : wtf::window < push_button, policy::has_size, policy::has_border, policy::has_cursor,
    policy::has_click, policy::has_text, policy::has_font, policy::has_paint, policy::has_mouse>
  {

    push_button() = delete;
    push_button(const push_button&) = delete;
    push_button &operator=(const push_button &) = delete;
    push_button(push_button&&) = delete;
    push_button &operator=(push_button&&) = delete;
    virtual ~push_button() = default;
    explicit push_button(HWND hParent) : window(hParent, true){
      border_style(border_styles::raised);
    }
    
  private:
    
    virtual void MouseMoveEvent(event_vkeys, const point& p) override{
      if (!_Down) return;
      if (!rect::get_client_rect(*this).is_in(p)){
        ::ReleaseCapture();
        _Down = false;
        border_style(border_styles::raised);
        refresh();
      }
    }
    virtual void MouseLButtonDownEvent(event_vkeys, const point&) override{
      _Down = true;
      border_style(border_styles::lowered);
      ::SetCapture(*this);
      refresh();
    }
    virtual void MouseLButtonUpEvent(event_vkeys, const point&) override{
      if (!_Down) return;
      _Down = false;
      border_style(border_styles::raised);
      refresh();
      ::ReleaseCapture();
    }

    virtual void PaintEvent(const device_context& dc, const paint_struct& ps){
      rect client = ps.rcPaint;
      client.top += border_width();
      client.left += border_width();
      client.bottom -= border_width();
      client.right -= border_width();
      if (_Down){
        client.top++;
        client.left++;
      }
      DrawText(dc, client);
    }


    bool _Down = false;
  };

}
