#pragma once

namespace wtf{
  namespace matrix_theme{


    struct button : wtf::window<button, wm_lbuttondown, wm_mousemove, wm_lbuttonup, wm_paint, has_cursor/*, has_label, has_font*/, has_show, has_move, has_background>{
      static const DWORD Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS;
      button(HWND hParent) : window(hParent){}

    private:


    };

  }
}