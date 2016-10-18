#pragma once

namespace wtf{
  namespace policy{
    /** has_dblclick
    * produces double click events
    */
    template<typename _SuperT, typename _ImplT>
    struct has_dblclick : _SuperT{

      callback<void(const mouse_event&)> OnDblClick;

    protected:
      has_dblclick(iwindow * pParent) : _SuperT(pParent){}

      virtual void DblClickEvent(const mouse_event& m){ OnDblClick(m); }

      LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool & bHandled) {
        if (WM_LBUTTONDBLCLK == umsg){
          mouse_event oEvent(mouse_event::buttons::left, static_cast<mouse_event::key_states>(wparam),
                             static_cast<int16_t>(LOWORD(lparam)), static_cast<int16_t>(HIWORD(lparam)));
          DblClickEvent(oEvent);
          bHandled = true;
        } else if (WM_RBUTTONDBLCLK == umsg){
          mouse_event oEvent(mouse_event::buttons::right, static_cast<mouse_event::key_states>(wparam),
                             static_cast<int16_t>(LOWORD(lparam)), static_cast<int16_t>(HIWORD(lparam)));
          DblClickEvent(oEvent);
          bHandled = true;
        } else if (WM_MBUTTONDBLCLK == umsg){
          mouse_event oEvent(mouse_event::buttons::middle, static_cast<mouse_event::key_states>(wparam),
                             static_cast<int16_t>(LOWORD(lparam)), static_cast<int16_t>(HIWORD(lparam)));
          DblClickEvent(oEvent);
          bHandled = true;
        }

        return 0;
      }

      bool _Down = false;
    };
  }
}
