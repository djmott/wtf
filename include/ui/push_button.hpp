#pragma once

namespace wtf{

    struct push_button : wtf::window<push_button, policy::has_size, policy::has_border,
      policy::has_click, policy::has_text, policy::has_font, policy::has_paint, policy::has_mouse>
    {
      push_button(HWND hParent) : window(hParent, true){}

      push_button() = delete;
      virtual ~push_button() = default;
      push_button(const push_button&) = delete;
      push_button &operator=(const push_button &) = delete;
      push_button(push_button&&) = delete;
      push_button &operator=(push_button&&) = delete;


    private:

    };

}
