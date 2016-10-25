#pragma once

namespace wtf {

    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::wm_size, _Policies...> : public window<_ImplT, _Policies...>{
      using __super_t = window<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window;
    public:


    protected:

      explicit window(iwindow * pParent) : __super_t(pParent){}

      virtual void on_wm_size(const point<coord_frame::client>&){}

      LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam){
        if (WM_SIZE == umsg) on_wm_size( point<coord_frame::client>(LOWORD(lparam), HIWORD(lparam)));
        return __super_t::handle_message(hwnd, umsg, wparam, lparam);
      }

    };

  }
