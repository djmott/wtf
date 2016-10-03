#pragma once

namespace wtf{
  namespace default_theme{
    struct button : wtf::window<button, has_size, has_button_border, has_click, has_text, has_paint, has_mouse_events>{
      button(HWND hParent) : window(hParent, true){}

      using button_states = window_type::button_states;
      using button_types = window_type::button_types;

    private:
    };
  }
}
