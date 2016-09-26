#pragma once

namespace wtf{
  namespace matrix_theme{

    struct form : window<form, WS_EX_OVERLAPPEDWINDOW, WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN, wm_close, wm_move, wm_moving, wm_mousemove, has_cursor, has_text, has_show, has_move, has_close>{
      form(HWND hParent = nullptr) : window(hParent){}
    };
  }
}