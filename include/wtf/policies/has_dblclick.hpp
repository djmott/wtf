#pragma once

namespace wtf{
  namespace policy{
    /** has_dblclick
    * produces double click events
    */
    template<typename _SuperT, typename _ImplT>
    struct has_dblclick : _SuperT{
      using mouse_msg_param = messages::mouse_msg_param;

    protected:
      virtual void wm_dblclick(const mouse_msg_param&){}

      has_dblclick(window<void> * pParent) : _SuperT(pParent){}


      LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool & bHandled) {
        if (WM_LBUTTONDBLCLK == umsg){
          wm_dblclick(mouse_msg_param(wparam, lparam, mouse_msg_param::buttons::left));
          bHandled = true;
        } else if (WM_MBUTTONDBLCLK == umsg){
          wm_dblclick(mouse_msg_param(wparam, lparam, mouse_msg_param::buttons::middle));
          bHandled = true;
        } else if (WM_RBUTTONDBLCLK == umsg){
          wm_dblclick(mouse_msg_param(wparam, lparam, mouse_msg_param::buttons::right));
          bHandled = true;
        }

        return 0;
      }

      bool _Down = false;
    };
  }
}
