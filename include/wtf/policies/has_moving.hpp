#pragma once

namespace wtf {
  namespace policy {

    template<typename _SuperT, typename _ImplT>
    struct has_moving : _SuperT {

    protected:
      virtual void wm_moving(rect<coord_frame::screen>&) {}

      has_moving(window<void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND , UINT umsg, WPARAM wparam, LPARAM lparam, bool & bHandled) {
        if (WM_MOVING == umsg){
          wm_moving(*reinterpret_cast<rect<coord_frame::screen>*>(lparam));
        }
        return 0;
      }

    };

  }
}
