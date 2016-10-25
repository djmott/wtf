#pragma once

namespace wtf {

    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::wm_timer, _Policies...> : public window<_ImplT, _Policies...>{
      using __super_t = window<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window;
    public:


    protected:
      explicit window(iwindow * pParent) : __super_t(pParent){}

      virtual void on_wm_timer(UINT_PTR) = 0;

      LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam){
        if (WM_TIMER == umsg) on_wm_timer(static_cast<UINT_PTR>(wparam));
        return __super_t::handle_message(hwnd, umsg, wparam, lparam);
      }

    };

  }
