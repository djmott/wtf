#pragma once

namespace wtf{
  
  namespace policy{
    /** has_mouse_move
    * produces mouse move events
    */
    template<typename _SuperT, typename _ImplT>
    struct has_mouse_move : _SuperT{

      callback<void(const mouse_event&)> OnMouseMove;
    protected:

      virtual void MouseMoveEvent(const mouse_event& m){ OnMouseMove(m); }

      virtual LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM lparam, bool &) override{
        if (WM_MOUSEMOVE == umsg){
          mouse_event oEvent(mouse_event::buttons::unspecified, static_cast<mouse_event::key_states>(wparam),
                         static_cast<int16_t>(LOWORD(lparam)), static_cast<int16_t>(HIWORD(lparam)));
          MouseMoveEvent(oEvent);
        }
        return 0;
      }
    };
  }
}
