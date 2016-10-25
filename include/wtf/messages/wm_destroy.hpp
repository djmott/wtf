#pragma once

namespace wtf{

  template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::wm_destroy, _Policies...> : public window<_ImplT, _Policies...>{
      using __super_t = window<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window;
    public:

    protected:
      template <typename, policy ... > friend class window;

      virtual void on_wm_destroy(){}

      explicit window(iwindow * pParent) : __super_t(pParent){}

      LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
        if (WM_DESTROY == umsg) on_wm_destroy();
        return __super_t::handle_message(hwnd, umsg, wparam, lparam);
      }

    };
  
}