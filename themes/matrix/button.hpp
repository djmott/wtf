#pragma once

namespace wtf{
  namespace matrix_theme{

    namespace _{
      template <WNDPROC _proc> using button_class = window_class_ex<_proc, 0, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, CS_GLOBALCLASS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW, icons::application, cursors::arrow, solid_brush<RGB(255, 255, 255)>, menus::null_menu, icons::application>;
    }

    struct button : wtf::window<button, _::button_class, wm_lbuttondown, wm_mousemove, wm_lbuttonup, wm_paint, has_label, has_font, has_show, has_move, has_background>{
      button(HWND hParent) : window(hParent){}

    private:


    };

  }
}