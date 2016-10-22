#pragma once

namespace wtf{
  namespace messages{

    template<typename _SuperT, typename>
    struct wm_killfocus : _SuperT{

    protected:

      virtual LRESULT on_wm_killfocus(HWND, bool&) = 0;

      wm_killfocus(window<void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM, bool& bhandled){
        if (WM_KILLFOCUS == umsg){
          return on_wm_killfocus(reinterpret_cast<HWND>(wparam), bhandled);
        }
        return 0;
      }
    };
  }
}
