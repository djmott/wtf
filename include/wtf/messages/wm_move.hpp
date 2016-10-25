#pragma once

namespace wtf {

    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::wm_move, _Policies...> : public window<_ImplT, _Policies...>{
      using __super_t = window<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window;
    public:

      enum class wm_size_flags {
        hide = SIZE_MAXHIDE,
        maximized = SIZE_MAXIMIZED,
        show = SIZE_MAXSHOW,
        minimized = SIZE_MINIMIZED,
        restored = SIZE_RESTORED,
      };

    protected:

      virtual void on_wm_move(const point<coord_frame::client>&, bool&){}

      explicit window(iwindow * pParent) : __super_t(pParent){}

      LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam){
        if (WM_MOVE == umsg) on_wm_move(point<coord_frame::client>(LOWORD(lparam), HIWORD(lparam)));
        return __super_t::handle_message(hwnd, umsg, wparam, lparam);
      }

    };

  }
