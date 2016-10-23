#pragma once

namespace wtf{
  namespace messages{

    template <typename _SuperT, typename>
    struct wm_nccalcsize : _SuperT{

      enum class activate_mode{
        active = WA_ACTIVE,
        click_active = WA_CLICKACTIVE,
        inactive = WA_INACTIVE,
      };

    protected:

      virtual LRESULT on_wm_nccalcsize(NCCALCSIZE_PARAMS *, bool&) = 0;
      virtual LRESULT on_wm_nccalcsize(RECT *, bool&) = 0;

      wm_nccalcsize(window<void> * pParent) : _SuperT(pParent){}
      
      LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM lparam, bool & handled){
        if (WM_NCCALCSIZE == umsg){
          if (wparam){
            return on_wm_nccalcsize(reinterpret_cast<NCCALCSIZE_PARAMS*>(lparam), handled);
          } else{
            return on_wm_nccalcsize(reinterpret_cast<RECT*>(lparam), handled);
          }
        }
        return 0;
      }

    };
  }
}