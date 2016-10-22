#pragma once

namespace wtf{

  namespace policy{
    /** has_mouse_down
    * produces mouse down events
    */
    template<typename _SuperT, typename _ImplT>
    struct has_mouse_down : _SuperT{
      using mouse_msg_param = messages::mouse_msg_param;

    protected:
      virtual void wm_mouse_down(const mouse_msg_param&){}

      has_mouse_down(window<void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM lparam, bool &) {
        if (WM_LBUTTONDOWN == umsg){
          wm_mouse_down(mouse_msg_param(wparam, lparam, mouse_msg_param::buttons::left));
        } else if (WM_MBUTTONDOWN == umsg){
          wm_mouse_down(mouse_msg_param(wparam, lparam, mouse_msg_param::buttons::middle));
        } else if (WM_RBUTTONDOWN == umsg){
            wm_mouse_down(mouse_msg_param(wparam, lparam, mouse_msg_param::buttons::right));
        }
        return 0;
      }
    };
  }
}
