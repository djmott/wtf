#pragma once

namespace wtf{

  namespace policy{
    /** has_mouse_up
    * produces mouse up events
    */
    template<typename _SuperT, typename _ImplT>
    struct has_mouse_up : _SuperT{

    protected:
      virtual void OnMouseUp(const mouse_event&){}

      has_mouse_up(window<void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM lparam, bool &) {
        if (WM_LBUTTONUP == umsg){
          OnMouseUp(mouse_event(wparam, lparam, mouse_event::buttons::left));
        } else if (WM_MBUTTONUP == umsg){
          OnMouseUp(mouse_event(wparam, lparam, mouse_event::buttons::middle));
        } else if (WM_RBUTTONUP == umsg){
          OnMouseUp(mouse_event(wparam, lparam, mouse_event::buttons::right));
        }
        return 0;
      }
    };
  }
}
