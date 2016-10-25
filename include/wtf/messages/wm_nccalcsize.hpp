#pragma once

namespace wtf{

    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::wm_nccalcsize, _Policies...> : public window<_ImplT, _Policies...>{
      using __super_t = window<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window;
    public:

      enum class activate_mode{
        active = WA_ACTIVE,
        click_active = WA_CLICKACTIVE,
        inactive = WA_INACTIVE,
      };

    protected:

      virtual LRESULT on_wm_nccalcsize(NCCALCSIZE_PARAMS *) = 0;
      virtual LRESULT on_wm_nccalcsize(RECT *) = 0;

      explicit window(iwindow * pParent) : __super_t(pParent){}

      LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam){
        if (WM_NCCALCSIZE == umsg){
          if (wparam) return on_wm_nccalcsize(reinterpret_cast<NCCALCSIZE_PARAMS*>(lparam));
          else return on_wm_nccalcsize(reinterpret_cast<RECT*>(lparam));
        }
        return __super_t::handle_message(hwnd, umsg, wparam, lparam);
      }

    };
  }
