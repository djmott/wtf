#pragma once

namespace wtf{

  namespace messages{

    template<typename _SuperT, typename _ImplT>
    struct wm_mouse_wheel : _SuperT{

    protected:

      virtual LRESULT on_wm_mouse_wheel(int16_t /*delta*/, const mouse_msg_param&, bool &) = 0{ return 0; }

      wm_mouse_wheel(window<void,void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM lparam, bool & bHandled) {
        if (WM_MOUSEWHEEL == umsg){
          return on_wm_mouse_wheel(static_cast<int16_t>(HIWORD(wparam)), mouse_msg_param(wparam, lparam, mouse_msg_param::buttons::unspecified), bHandled);
        }
        return 0;
      }
    };
  }
}
