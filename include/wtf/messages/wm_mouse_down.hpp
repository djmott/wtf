#pragma once

namespace wtf{

  namespace messages{

    template<typename _SuperT, typename _ImplT>
    struct wm_mouse_down : _SuperT{


    protected:

      virtual LRESULT on_wm_mouse_down(const mouse_msg_param&, bool &) = 0;

      wm_mouse_down(window<void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM lparam, bool & bHandled) {
        if (WM_LBUTTONDOWN == umsg){
          return on_wm_mouse_down(mouse_msg_param(wparam, lparam, mouse_msg_param::buttons::left), bHandled);
        } else if (WM_MBUTTONDOWN == umsg){
          return on_wm_mouse_down(mouse_msg_param(wparam, lparam, mouse_msg_param::buttons::middle), bHandled);
        } else if (WM_RBUTTONDOWN == umsg){
          return on_wm_mouse_down(mouse_msg_param(wparam, lparam, mouse_msg_param::buttons::right), bHandled);
        }
        return 0;
      }
    };
  }
}
