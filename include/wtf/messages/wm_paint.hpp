#pragma once

namespace wtf {
  namespace policy {

    template<typename _SuperT, typename _ImplT>
    struct wm_paint : _SuperT {

    protected:
      virtual LRESULT on_wm_paint(const device_context&, const paint_struct&, bool&) = 0;

      wm_paint(window<void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND , UINT umsg, WPARAM wparam, LPARAM lparam, bool &bhandled) {
        if (WM_PAINT == umsg){
          return on_wm_paint(*reinterpret_cast<const device_context *>(wparam),
                             *reinterpret_cast<const paint_struct *>(lparam), bhandled);
        }
      }

    };

  }
}
