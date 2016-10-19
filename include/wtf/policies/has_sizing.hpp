#pragma once

namespace wtf {
  namespace policy {

    template<typename _SuperT, typename _ImplT>
    struct has_sizing : _SuperT {

    protected:
      virtual void wm_sizing(rect<coord_frame::screen>&) {}


      has_sizing(window<void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND , UINT umsg, WPARAM wparam, LPARAM lparam, bool & bHandled) {
        if (WM_SIZING == umsg){
          wm_sizing(*reinterpret_cast<rect<coord_frame::screen>*>(lparam));
          lret = TRUE;
        }
        return 0;
      }

    };

  }
}
