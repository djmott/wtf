#pragma once

namespace wtf{
  struct rect : RECT{

    rect() = default;

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