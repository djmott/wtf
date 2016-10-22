#pragma once

namespace wtf{

  namespace policy{
    /** has_mouse_wheel
    * produces mouse wheel events
    */
    template<typename _SuperT, typename _ImplT>
    struct has_mouse_wheel : _SuperT{
      using mouse_msg_param = messages::mouse_msg_param;

    protected:
      virtual void wm_mouse_wheel(int16_t /*delta*/, const mouse_msg_param&){}

      has_mouse_wheel(window<void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM lparam, bool &) {
        if (WM_MOUSEWHEEL == umsg){
          wm_mouse_wheel(static_cast<int16_t>(HIWORD(wparam)), mouse_msg_param(wparam, lparam, mouse_msg_param::buttons::unspecified));
        }
        return 0;
      }
    };
  }
}
