#pragma once

namespace wtf{
  namespace default_theme{
    struct form : window<form, WS_EX_OVERLAPPEDWINDOW, WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN, wm_close, wm_move, wm_moving, wm_mousemove, has_cursor, has_text, has_show, has_move, has_close, has_border, has_background>{
      form() : window(nullptr){}
      virtual ~form() = default;
    };
  }
}

