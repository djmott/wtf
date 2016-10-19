#pragma once

namespace wtf{

  namespace policy{
    /** has_mouse_up
    * produces mouse up events
    */
    template<typename _SuperT, typename _ImplT>
    struct has_mouse_up : _SuperT{

    protected:
      virtual void wm_mouse_up(const mouse_event&){}

      has_mouse_up(window<void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM lparam, bool &) {
        if (WM_LBUTTONUP == umsg){
          wm_mouse_up(mouse_event(wparam, lparam, mouse_event::buttons::left));
        } else if (WM_MBUTTONUP == umsg){
          wm_mouse_up(mouse_event(wparam, lparam, mouse_event::buttons::middle));
        } else if (WM_RBUTTONUP == umsg){
          wm_mouse_up(mouse_event(wparam, lparam, mouse_event::buttons::right));
        }
        return 0;
      }
    };
  }
}
