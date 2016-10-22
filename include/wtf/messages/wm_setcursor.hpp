#pragma once

namespace wtf {
  namespace messages{

    template<typename _SuperT, typename _ImplT>
    struct wm_setcursor : _SuperT {

      enum class wm_nchittest_flags {
        error = HTERROR,
        transparent = HTTRANSPARENT,
        nowhere = HTNOWHERE,
        client = HTCLIENT,
        caption = HTCAPTION,
        sysmenu = HTSYSMENU,
        growbox = HTGROWBOX,
        size = HTSIZE,
        menu = HTMENU,
        hscroll = HTHSCROLL,
        vscroll = HTVSCROLL,
        minbutton = HTMINBUTTON,
        maxbutton = HTMAXBUTTON,
        left = HTLEFT,
        right = HTRIGHT,
        top = HTTOP,
        topleft = HTTOPLEFT,
        topright = HTTOPRIGHT,
        bottom = HTBOTTOM,
        bottomleft = HTBOTTOMLEFT,
        bottomright = HTBOTTOMRIGHT,
        border = HTBORDER,
        reduce = HTREDUCE,
        zoom = HTZOOM,
        sizefirst = HTSIZEFIRST,
        sizelast = HTSIZELAST,
        object = HTOBJECT,
        close = HTCLOSE,
        help = HTHELP,
      };
    protected:

      virtual LRESULT on_wm_setcursor(wm_nchittest_flags, bool&) = 0;

      wm_setcursor(window<void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND , UINT umsg, WPARAM , LPARAM lparam, bool &bhandled) {
        if (WM_SETCURSOR == umsg) {
          return on_wm_setcursor(static_cast<wm_nchittest_flags>(LOWORD(lparam)), bhandled);
        }
        return 0;
      }
    };
  }
}
