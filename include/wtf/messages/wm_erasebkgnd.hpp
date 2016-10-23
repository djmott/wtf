#pragma once

namespace wtf {
  namespace messages {

    template<typename _SuperT, typename _ImplT>
    struct wm_erasebkgnd: _SuperT {


    protected:

      virtual LRESULT on_wm_erasebkgnd(const device_context& , const rect<coord_frame::client>& , bool&) = 0{ return 0; }

      wm_erasebkgnd(window<void,void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND , UINT umsg, WPARAM wparam, LPARAM lparam, bool & bHandled) {
        if (WM_ERASEBKGND == umsg){
          auto &oDC = *reinterpret_cast<const device_context *>(lparam);
          return on_wm_erasebkgnd(oDC, rect<coord_frame::client>::get(*this), bHandled);
        }
        return 0;
      }

    };

  }
}
