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

      virtual void DblClickEvent(const mouse_event& m){ OnDblClick(m); }

      virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool &) override{
        if (WM_LBUTTONDBLCLK == umsg){
          mouse_event oEvent(mouse_event::buttons::left, static_cast<mouse_event::key_states>(wparam),
                             static_cast<int16_t>(LOWORD(lparam)), static_cast<int16_t>(HIWORD(lparam)));
          DblClickEvent(oEvent);
        } else if (WM_RBUTTONDBLCLK == umsg){
          mouse_event oEvent(mouse_event::buttons::right, static_cast<mouse_event::key_states>(wparam),
                             static_cast<int16_t>(LOWORD(lparam)), static_cast<int16_t>(HIWORD(lparam)));
          DblClickEvent(oEvent);
        } else if (WM_MBUTTONDBLCLK == umsg){
          mouse_event oEvent(mouse_event::buttons::middle, static_cast<mouse_event::key_states>(wparam),
                             static_cast<int16_t>(LOWORD(lparam)), static_cast<int16_t>(HIWORD(lparam)));
          DblClickEvent(oEvent);
        }

        return 0;
      }

      bool _Down = false;
    };
  }
}
