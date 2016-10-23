#pragma once

namespace wtf {
  namespace messages {

    template<typename _SuperT, typename>
    struct wm_setfocus : _SuperT {

    protected:

      virtual LRESULT on_wm_setfocus(HWND, bool&) = 0{ return 0; }

      wm_setfocus(window<void,void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM, bool& bhandled) {
        if (WM_SETFOCUS == umsg) {
          return on_wm_setfocus(reinterpret_cast<HWND>(wparam), bhandled);
        }
        return 0;
      }
    };
  }
}
