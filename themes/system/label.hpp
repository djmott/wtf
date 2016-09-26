#pragma once
namespace wtf{
  namespace system_theme{
    struct label : base_window<label, has_cursor, has_background, has_move, has_show, has_border>{
      label(HWND hParent) : base_window(){ 
        _handle = wtf::exception::throw_lasterr_if(
          ::CreateWindowEx(0, window_class_type::get().name(), nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hParent, nullptr, reinterpret_cast<HINSTANCE>(&__ImageBase), this),
          [](HWND h){ return nullptr == h; });
      }
      virtual ~label() = default;
    };
  }
}