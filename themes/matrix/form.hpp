#pragma once

namespace wtf{
  namespace matrix_theme{

    namespace _{
      template <WNDPROC _proc> using form_class = window_class_ex<_proc, WS_EX_OVERLAPPEDWINDOW, WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPSIBLINGS, CS_GLOBALCLASS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW, icons::application, cursors::arrow, solid_brush<RGB(0, 0, 0)>, menus::null_menu, icons::application>;
    }


    struct form : window<form, _::form_class, wm_close, wm_move, wm_moving, wm_mousemove, has_titlebar, has_show, has_move, has_close>{
      form(HWND hParent = nullptr) : window(hParent){}
    };
  }
}