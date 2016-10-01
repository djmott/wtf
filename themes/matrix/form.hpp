#pragma once

namespace wtf{
  namespace matrix_theme{

    struct form : window<form, wm_close, wm_move, wm_moving, wm_mousemove, has_cursor, has_titlebar, has_show, has_move, has_close>{
      static const DWORD ExStyle = WS_EX_OVERLAPPEDWINDOW;
      static const DWORD Style = WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN;
      form(HWND hParent = nullptr) : window(hParent){}
    };
  }
}