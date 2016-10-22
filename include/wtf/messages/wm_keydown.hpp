#pragma once

namespace wtf{
  namespace messages{

    template <typename _SuperT, typename _ImplT>
    struct wm_keydown : _SuperT{

    protected:
      virtual LRESULT on_wm_keydown(TCHAR char_code, keyboard_msg_param, bool&) = 0;

      wm_keydown(window<void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM lparam, bool&){
        if (WM_KEYDOWN == umsg){
          return on_wm_char(static_cast<UINT>(wparam), *reinterpret_cast<keyboard_msg_param*>(&lparam), bHandled);
        }
        return 0;
      }


    };
  }
}