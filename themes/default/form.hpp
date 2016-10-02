#pragma once

namespace wtf{
  namespace default_theme{
    struct form : window<form, has_show, has_cursor, has_titlebar, has_size, has_close, has_background>{
      static const DWORD ExStyle = WS_EX_OVERLAPPEDWINDOW;
      static const DWORD Style = WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN;
      form(HWND hParent=nullptr) : window(hParent, true){}
      virtual ~form() = default;

    protected:

    };
  }
}

