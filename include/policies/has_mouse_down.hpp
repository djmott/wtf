#pragma once

namespace wtf{

  namespace policy{
    /** has_mouse_down
    * produces mouse down events
    */
    template<typename _SuperT, typename _ImplT>
    struct has_mouse_down : _SuperT{

      callback<void(const mouse_event&)> OnMouseDown;

    protected:

      virtual void MouseDownEvent(const mouse_event& m){ OnMouseDown(m); }

      virtual LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM lparam, bool &) override{
        if (WM_LBUTTONDOWN == umsg){
          mouse_event oEvent(mouse_event::buttons::left, static_cast<mouse_event::key_states>(wparam),
                              static_cast<int16_t>(LOWORD(lparam)), static_cast<int16_t>(HIWORD(lparam)));
          MouseDownEvent(oEvent);
        } else if (WM_RBUTTONDOWN == umsg){
            mouse_event oEvent(mouse_event::buttons::right, static_cast<mouse_event::key_states>(wparam),
                                static_cast<int16_t>(LOWORD(lparam)), static_cast<int16_t>(HIWORD(lparam)));
            MouseDownEvent(oEvent);
        } else if (WM_MBUTTONDOWN == umsg){
          mouse_event oEvent(mouse_event::buttons::middle, static_cast<mouse_event::key_states>(wparam),
                              static_cast<int16_t>(LOWORD(lparam)), static_cast<int16_t>(HIWORD(lparam)));
          MouseDownEvent(oEvent);
        }
        return 0;
      }
    };
  }
}
