#pragma once

namespace wtf{
  
  namespace policy{
    /** has_mouse_move
    * produces mouse move events
    */
    template<typename _SuperT, typename _ImplT>
    struct has_mouse_move : _SuperT{



    protected:
      virtual void wm_mouse_move(const mouse_event&){}

      has_mouse_move(window<void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM lparam, bool &) {
        if (WM_MOUSEMOVE == umsg){
          wm_mouse_move(mouse_event(wparam, lparam, mouse_event::buttons::unspecified));
        }
        return 0;
      }
    };
  }
}
