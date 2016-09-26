#pragma once

namespace wtf{
  namespace system_theme{


    struct button : wtf::base_window<button, wm_lbuttondown, wm_mousemove, wm_lbuttonup, wm_paint, has_cursor, has_text,  has_show, has_move, has_background>{
      button(HWND hParent) : base_window(){
        _handle = wtf::exception::throw_lasterr_if(
          ::CreateWindowEx(0, L"BUTTON", nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hParent, nullptr, reinterpret_cast<HINSTANCE>(&__ImageBase), this),
          [](HWND h){ return nullptr == h; });
      }

    private:


    };

  }
}