#pragma once

namespace wtf {
  namespace policy {
    /** has_icon
    * Provides an icon associated with the window
    */
    template<typename _SuperT, typename _ImplT>
    struct has_icon : _SuperT {

      virtual icon &big_icon() { return _big_icon; }

      virtual const icon &big_icon() const { return _big_icon; }

      void big_icon(icon &&src) { _big_icon = std::move(src); }

      virtual icon &small_icon() { return _small_icon; }

      virtual const icon &small_icon() const { return _small_icon; }

      void small_icon(icon &&src) { _small_icon = std::move(src); }

    protected:
      has_icon(iwindow * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND , UINT umsg, WPARAM wparam, LPARAM , bool &bhandled) {
        if (WM_GETICON != umsg) return 0;
        bhandled = true;
        if (ICON_BIG == wparam) return reinterpret_cast<LRESULT>(static_cast<HICON>(big_icon()));
        else return reinterpret_cast<LRESULT>(static_cast<HICON>(small_icon()));
      }

      icon _big_icon = icon::from_system(icon::system_icons::winlogo);
      icon _small_icon = icon::from_system(icon::system_icons::winlogo);
    };
  }
}
