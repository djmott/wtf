#pragma once

namespace wtf{
  namespace messages{

    template <typename _SuperT, typename _ImplT>
    struct wm_keyup : _SuperT{

    protected:

      virtual LRESULT on_wm_keyup(UINT char_code, keyboard_msg_param, bool&) = 0;

      wm_keyup(window<void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bHandled){
        if (WM_KEYUP==umsg) return on_wm_keyup(static_cast<UINT>(wparam), *reinterpret_cast<keyboard_msg_param*>(&lparam), bHandled);
        return 0;
      }

    };
  }
}
