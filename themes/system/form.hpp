#pragma once

namespace wtf{
  namespace system_theme{

    struct form : base_window<form, wm_close, wm_move, wm_moving, wm_mousemove, has_cursor, has_text, has_show, has_move, has_close, has_background>{
      form(HWND hParent = nullptr) : base_window(){
        _handle = wtf::exception::throw_lasterr_if(
          ::CreateWindowEx( WS_EX_OVERLAPPEDWINDOW, window_class_type::get().name(), nullptr, WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hParent, nullptr, reinterpret_cast<HINSTANCE>(&__ImageBase), this),
          [](HWND h){ return nullptr == h; });
      }
    };
  }
}