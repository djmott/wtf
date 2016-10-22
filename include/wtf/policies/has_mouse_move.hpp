#pragma once

namespace wtf{
  
  namespace policy{
    /** has_mouse_move
    * produces mouse move events
    */
    template<typename _SuperT, typename _ImplT>
    struct has_mouse_move : _SuperT{

      using mouse_msg_param = messages::mouse_msg_param;

    protected:
      virtual void wm_mouse_move(const mouse_msg_param&){}

      has_mouse_move(window<void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM lparam, bool &) {
        if (WM_MOUSEMOVE == umsg){
          wm_mouse_move(mouse_msg_param(wparam, lparam, mouse_msg_param::buttons::unspecified));
        }
        return 0;
      }
    };
  }
}
