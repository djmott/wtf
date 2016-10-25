#pragma once

namespace wtf {

    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::wm_sizing, _Policies...> : public window<_ImplT, _Policies...>{
      using __super_t = window<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window;
    public:

    protected:
      virtual void on_wm_sizing(rect<coord_frame::screen>&){}

      explicit window(iwindow * pParent) : __super_t(pParent){}

      LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam){
        if (WM_SIZING == umsg) on_wm_sizing(*reinterpret_cast<rect<coord_frame::screen>*>(lparam), bHandled);
        return __super_t::handle_message(hwnd, umsg, wparam, lparam);
      }

    };

  }
