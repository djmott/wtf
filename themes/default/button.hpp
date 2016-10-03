#pragma once

namespace wtf{
  namespace default_theme{
    struct button : wtf::window<button, has_size, has_background, has_button_border, has_click, has_text, has_paint, has_mouse_events>{
      button(HWND hParent) : window(hParent, true){}

    private:
    };
  }
}