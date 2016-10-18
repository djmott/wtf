#pragma once

namespace wtf{

  namespace policy{
    /** has_mouse_up
    * produces mouse up events
    */
    template<typename _SuperT, typename _ImplT>
    struct has_mouse_up : _SuperT{

      callback<void(const mouse_event&)> OnMouseUp;

    protected:
      has_mouse_up(iwindow * pParent) : _SuperT(pParent){}

      virtual void MouseUpEvent(const mouse_event& m){ OnMouseUp(m); }

      LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM lparam, bool &) {
        if (WM_LBUTTONUP == umsg){
          mouse_event oEvent(mouse_event::buttons::left, static_cast<mouse_event::key_states>(wparam),
                              static_cast<int16_t>(LOWORD(lparam)), static_cast<int16_t>(HIWORD(lparam)));
          MouseUpEvent(oEvent);
        } else if (WM_RBUTTONUP == umsg){
          mouse_event oEvent(mouse_event::buttons::right, static_cast<mouse_event::key_states>(wparam),
                              static_cast<int16_t>(LOWORD(lparam)), static_cast<int16_t>(HIWORD(lparam)));
          MouseUpEvent(oEvent);
        } else if (WM_MBUTTONUP == umsg){
          mouse_event oEvent(mouse_event::buttons::middle, static_cast<mouse_event::key_states>(wparam),
                              static_cast<int16_t>(LOWORD(lparam)), static_cast<int16_t>(HIWORD(lparam)));
          MouseUpEvent(oEvent);
        }
        return 0;
      }
    };
  }
}
