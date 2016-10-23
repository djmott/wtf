#pragma once

namespace wtf {
  namespace messages {

    template<typename _SuperT, typename _ImplT>
    struct wm_size : _SuperT {


    protected:

      explicit wm_size(window<void,void> * pParent) : _SuperT(pParent){}

      virtual LRESULT on_wm_size(const point<coord_frame::client>&, bool&) = 0{ return 0; }

      LRESULT handle_message(HWND , UINT umsg, WPARAM wparam, LPARAM lparam, bool & bHandled) {
        if (WM_SIZE == umsg) return on_wm_size( point<coord_frame::client>(LOWORD(lparam), HIWORD(lparam)), bHandled);
        return 0;
      }

    };

  }
}
