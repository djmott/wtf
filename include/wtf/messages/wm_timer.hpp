#pragma once

namespace wtf {
  namespace policy {

    template<typename _SuperT, typename _ImplT>
    struct wm_timer : _SuperT {


    protected:

      virtual LRESULT on_wm_timer(UINT_PTR, bool&) = 0;

      LRESULT handle_message(HWND , UINT umsg, WPARAM wparam, LPARAM , bool & bHandled) {
        if (WM_TIMER == umsg) return on_wm_timer(static_cast<UINT_PTR>(wparam), bHandled);
        return 0;
      }

    };

  }
}
