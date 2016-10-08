#pragma once

namespace wtf{

  struct push_button : wtf::window < push_button, policy::has_button_border, policy::has_size, policy::has_cursor
    , policy::has_text, policy::has_font, policy::has_paint, policy::has_click>
  {

    push_button() = delete;
    push_button(const push_button&) = delete;
    push_button &operator=(const push_button &) = delete;
    push_button(push_button&&) = delete;
    push_button &operator=(push_button&&) = delete;
    virtual ~push_button() = default;
    explicit push_button(HWND hParent) : window(hParent, true){}
    
  };

}
