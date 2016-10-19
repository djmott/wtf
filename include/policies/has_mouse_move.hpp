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
      has_mouse_move(iwindow * pParent) : _SuperT(pParent){}


      LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM lparam, bool &) {
        if (WM_MOUSEMOVE == umsg){
          OnMouseMove(mouse_event(wparam, lparam, mouse_event::buttons::unspecified));
        }
        return 0;
      }
    };
  }
}
