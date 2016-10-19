#pragma once

namespace wtf{

  namespace policy{
    /** has_mouse_wheel
    * produces mouse wheel events
    */
    template<typename _SuperT, typename _ImplT>
    struct has_mouse_wheel : _SuperT{


    protected:
      virtual void wm_mouse_wheel(int16_t /*delta*/, const mouse_event&){}

      has_mouse_wheel(window<void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM lparam, bool &) {
        if (WM_MOUSEWHEEL == umsg){
          wm_mouse_wheel(static_cast<int16_t>(HIWORD(wparam)), mouse_event(wparam, lparam, mouse_event::buttons::unspecified));
        }
        return 0;
      }
    };
  }
}
