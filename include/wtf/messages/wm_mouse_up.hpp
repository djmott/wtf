#pragma once

namespace wtf{

  namespace messages{

    template<typename _SuperT, typename _ImplT>
    struct wm_mouse_up : _SuperT{

    protected:
      virtual LRESULT on_wm_mouse_up(const mouse_msg_param&, bool&) = 0;

      wm_mouse_up(window<void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM lparam, bool & bHandled) {
        if (WM_LBUTTONUP == umsg){
          return on_wm_mouse_up(mouse_msg_param(wparam, lparam, mouse_msg_param::buttons::left), bHandled);
        } else if (WM_MBUTTONUP == umsg){
          return on_wm_mouse_up(mouse_msg_param(wparam, lparam, mouse_msg_param::buttons::middle), bHandled);
        } else if (WM_RBUTTONUP == umsg){
          return on_wm_mouse_up(mouse_msg_param(wparam, lparam, mouse_msg_param::buttons::right), bHandled);
        }
        return 0;
      }
    };
  }
}
