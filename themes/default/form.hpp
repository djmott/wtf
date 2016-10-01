#pragma once

namespace wtf{
  namespace default_theme{
    struct form : window<form, wm_paint, has_show, wm_close, wm_move, wm_moving, wm_mousemove, has_cursor, has_text, has_move, has_close, has_background>{
      static const DWORD ExStyle = WS_EX_OVERLAPPEDWINDOW;
      static const DWORD Style = WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN;
      form() : window(nullptr){}
      virtual ~form() = default;
    };
  }
}

