#pragma once

namespace wtf{
  namespace policy{
    /** has_dblclick
    * produces double click events
    */
    template<typename _SuperT, typename _ImplT>
    struct has_dblclick : _SuperT{


    protected:
      virtual void OnDoubleClick(const mouse_event&){}

      has_dblclick(window<void> * pParent) : _SuperT(pParent){}


      LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool & bHandled) {
        if (WM_LBUTTONDBLCLK == umsg){
          OnDoubleClick(mouse_event(wparam, lparam, mouse_event::buttons::left));
          bHandled = true;
        } else if (WM_MBUTTONDBLCLK == umsg){
          OnDoubleClick(mouse_event(wparam, lparam, mouse_event::buttons::middle));
          bHandled = true;
        } else if (WM_RBUTTONDBLCLK == umsg){
          OnDoubleClick(mouse_event(wparam, lparam, mouse_event::buttons::right));
          bHandled = true;
        }

        return 0;
      }

      bool _Down = false;
    };
  }
}
