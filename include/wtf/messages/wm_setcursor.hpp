#pragma once

namespace wtf {
  namespace messages{

    template<typename _SuperT, typename _ImplT>
    struct wm_setcursor : _SuperT {


    protected:

      virtual LRESULT on_wm_setcursor(wm_nchittest_flags, bool&) = 0;

      wm_setcursor(window<void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND , UINT umsg, WPARAM , LPARAM lparam, bool &bhandled) {
        if (WM_SETCURSOR == umsg) {
          return on_wm_setcursor(static_cast<wm_nchittest_flags>(LOWORD(lparam)), bhandled);
        }
        return 0;
      }
    };
  }
}
