#pragma once

namespace wtf{

  template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::wm_activate, _Policies...> : public window<_ImplT, _Policies...>{
      using __super_t = window<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window;
    public:

      enum class activate_mode{
        active = WA_ACTIVE,
        click_active = WA_CLICKACTIVE,
        inactive = WA_INACTIVE,
      };

    protected:
      template <typename, policy ... > friend class window;

      virtual void on_wm_activate(activate_mode, bool minimized, HWND target){}

      explicit window(iwindow * pParent) : __super_t(pParent){}

      LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
        if (WM_ACTIVATE == umsg) on_wm_activate(static_cast<activate_mode>(LOWORD(wparam)), HIWORD(wparam)?true:false, reinterpret_cast<HWND>(LPARAM));
        return __super_t::handle_message(hwnd, umsg, wparam, lparam);
      }

    };
  
}