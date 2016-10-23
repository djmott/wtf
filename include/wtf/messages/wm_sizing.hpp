#pragma once

namespace wtf {
  namespace messages{

    template<typename _SuperT, typename _ImplT>
    struct wm_sizing : _SuperT {

    protected:
      virtual LRESULT on_wm_sizing(rect<coord_frame::screen>&, bool&) = 0{ return 0; }

      wm_sizing(window<void,void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND , UINT umsg, WPARAM wparam, LPARAM lparam, bool & bHandled) {
        if (WM_SIZING == umsg){
          return on_wm_sizing(*reinterpret_cast<rect<coord_frame::screen>*>(lparam), bHandled);
        }
        return 0;
      }

    };

  }
}
