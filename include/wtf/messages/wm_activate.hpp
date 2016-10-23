#pragma once

namespace wtf{
  namespace messages{
    template <typename _SuperT, typename>
    struct wm_activate : _SuperT{

      enum class activate_mode{
        active = WA_ACTIVE,
        click_active = WA_CLICKACTIVE,
        inactive = WA_INACTIVE,
      };

    protected:

      virtual LRESULT on_wm_activate(activate_mode, bool minimized, HWND target, bool& handled) = 0{ return 0; }

      wm_activate(window<void,void> * pParent) : _SuperT(pParent){}


      LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM, bool &) {
        if (WM_ACTIVATE == umsg){
          return on_wm_activate(static_cast<activate_mode>(LOWORD(wparam)), HIWORD(wparam)?true:false, reinterpret_cast<HWND>(LPARAM), handled);
        }
        return 0;
      }

    };
  }
}