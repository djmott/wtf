#pragma once

namespace wtf{
  struct rect : RECT{

    rect() = default;

    rect(const RECT& src){ memcpy(this, &src, sizeof(RECT)); }

    static rect get_window_rect(HWND hwnd){
      rect oRet;
      wtf::exception::throw_lasterr_if(GetWindowRect(hwnd, &oRet), [](BOOL b){return !b; });
      return oRet;
    }
    static rect get_client_rect(HWND hwnd){
      rect oRet;
      wtf::exception::throw_lasterr_if(GetClientRect(hwnd, &oRet), [](BOOL b){return !b; });
      return oRet;
    }


  };
}