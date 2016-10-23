#pragma once

namespace wtf {
  namespace policy {

    template<typename _SuperT, typename _ImplT>
    struct wm_moving : _SuperT {

    protected:
      virtual LRESULT on_wm_moving(rect<coord_frame::screen>&, bool & ) = 0{ return 0; }

      wm_moving(window<void,void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND , UINT umsg, WPARAM wparam, LPARAM lparam, bool & bHandled) {
        if (WM_MOVING == umsg){
          return on_wm_moving(*reinterpret_cast<rect<coord_frame::screen>*>(lparam), bHandled);
        }
        return 0;
      }

    };

  }
}
