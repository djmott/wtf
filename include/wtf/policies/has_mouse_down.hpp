#pragma once

namespace wtf{

  namespace policy{
    /** has_mouse_down
    * produces mouse down events
    */
    template<typename _SuperT, typename _ImplT>
    struct has_mouse_down : _SuperT{


    protected:
      virtual void OnMouseDown(const mouse_event&){}

      has_mouse_down(window<void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM lparam, bool &) {
        if (WM_LBUTTONDOWN == umsg){
          OnMouseDown(mouse_event(wparam, lparam, mouse_event::buttons::left));
        } else if (WM_MBUTTONDOWN == umsg){
          OnMouseDown(mouse_event(wparam, lparam, mouse_event::buttons::middle));
        } else if (WM_RBUTTONDOWN == umsg){
            OnMouseDown(mouse_event(wparam, lparam, mouse_event::buttons::right));
        }
        return 0;
      }
    };
  }
}
