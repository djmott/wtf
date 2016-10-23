#pragma once

namespace wtf {
  namespace messages {

    enum class icon_type{
      big_icon = ICON_BIG,
      small_icon = ICON_SMALL,
      small2_icon = ICON_SMALL2,
    };


    template<typename _SuperT, typename _ImplT>
    struct wm_geticon : _SuperT {

    protected:

      virtual HICON on_wm_geticon(icon_type,bool&) = 0{ return nullptr; }

      explicit wm_geticon(window<void,void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM, bool &bhandled) {
        if (WM_GETICON == umsg){
          return reinterpret_cast<LRESULT>(on_wm_geticon(static_cast<icon_type>(wparam), bhandled));
        }
        return 0;
      }
    };
  }
}
