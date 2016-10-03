#pragma once

namespace wtf{
  namespace default_theme{
    struct form : window<form, has_icon, has_show, has_cursor, has_titlebar, has_size, has_close, has_paint, has_mouse_events, has_click>{
      static const DWORD ExStyle = WS_EX_OVERLAPPEDWINDOW | WS_EX_CONTROLPARENT;
      static const DWORD Style = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
      form(HWND hParent = nullptr) : window(hParent, true){}
      virtual ~form() = default;

    protected:
    };
  }
}
