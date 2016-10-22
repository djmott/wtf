#pragma once

namespace wtf{
  
  namespace messages{

    template<typename _SuperT, typename _ImplT>
    struct wm_mouse_move : _SuperT{

    protected:
      virtual LRESULT on_wm_mouse_move(const mouse_msg_param&, bool&) = 0;

      wm_mouse_move(window<void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM lparam, bool & bHandled) {
        if (WM_MOUSEMOVE == umsg){
          return on_wm_mouse_move(mouse_msg_param(wparam, lparam, mouse_msg_param::buttons::unspecified), bHandled);
        }
        return 0;
      }
    };
  }
}
