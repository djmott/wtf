#pragma once

namespace wtf{

  namespace policy{
    /** has_mouse_wheel
    * produces mouse wheel events
    */
    template<typename _SuperT, typename _ImplT>
    struct has_mouse_wheel : _SuperT{

      callback<void(int16_t /*delta*/, const mouse_event&)> OnMouseWheel;

    protected:
      has_mouse_wheel(iwindow * pParent) : _SuperT(pParent){}

      virtual void MouseWheelEvent(int16_t delta, const mouse_event& m){ OnMouseWheel(delta, m); }

      LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM lparam, bool &) {
        if (WM_MOUSEWHEEL == umsg){
          mouse_event oEvent(mouse_event::buttons::unspecified, static_cast<mouse_event::key_states>(LOWORD(wparam)),
                              static_cast<int16_t>(LOWORD(lparam)), static_cast<int16_t>(HIWORD(lparam)));
          MouseWheelEvent(static_cast<int16_t>(HIWORD(wparam)), oEvent);
        }
        return 0;
      }
    };
  }
}
