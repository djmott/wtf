#pragma once

namespace wtf {
  namespace messages{

    template<typename _SuperT, typename _ImplT>
    struct wm_move : _SuperT {

      enum class wm_size_flags {
        hide = SIZE_MAXHIDE,
        maximized = SIZE_MAXIMIZED,
        show = SIZE_MAXSHOW,
        minimized = SIZE_MINIMIZED,
        restored = SIZE_RESTORED,
      };

    protected:

      virtual LRESULT on_wm_move(const point<coord_frame::client>&, bool&) = 0;

      wm_move(window<void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM lparam, bool & bHandled){
        if (WM_MOVE == umsg){
          return on_wm_move(point<coord_frame::client>(LOWORD(lparam), HIWORD(lparam)), bHandled);
        }
        return 0;
      }

    };

  }
}
