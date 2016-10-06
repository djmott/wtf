#pragma once

namespace wtf {
  namespace policy {
    /** has_cursor
    * Provides mouse pointer customization
    */
    template<typename _SuperT, typename _ImplT>
    struct has_cursor : _SuperT {
      virtual ~has_cursor() = default;

      has_cursor() = default;
      has_cursor(const has_cursor&) = delete;
      has_cursor &operator=(const has_cursor &) = delete;
      has_cursor(has_cursor&&) = delete;
      has_cursor &operator=(has_cursor&&) = delete;

      virtual const wtf::cursor &cursor_pointer() const { return cursor::global(cursor::style::arrow); }

      virtual const wtf::cursor &cursor_size_ns() const { return cursor::global(cursor::style::size_ns); }

      virtual const wtf::cursor &cursor_size_we() const { return cursor::global(cursor::style::size_we); }

      virtual const wtf::cursor &cursor_size_nwse() const { return cursor::global(cursor::style::size_nwse); }

      virtual const wtf::cursor &cursor_size_nesw() const { return cursor::global(cursor::style::size_nesw); }


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

      virtual void OnSetCursor(wm_nchittest_flags flags) {
        switch (flags) {
          case wm_nchittest_flags::top:
          case wm_nchittest_flags::bottom:
            SetCursor(cursor_size_ns());
            break;
          case wm_nchittest_flags::bottomleft:
          case wm_nchittest_flags::topright:
            SetCursor(cursor_size_nesw());
            break;
          case wm_nchittest_flags::bottomright:
          case wm_nchittest_flags::topleft:
            SetCursor(cursor_size_nwse());
            break;
          case wm_nchittest_flags::left:
          case wm_nchittest_flags::right:
            SetCursor(cursor_size_we());
            break;
          default:
            SetCursor(cursor_pointer());
            break;
        }
      }

      virtual LRESULT handle_message(HWND , UINT umsg, WPARAM , LPARAM lparam, bool &bhandled) override {
        if (WM_SETCURSOR == umsg) {
          OnSetCursor(static_cast<wm_nchittest_flags>(LOWORD(lparam)));
          bhandled = true;
          return TRUE;
        }
        return 0;
      }
    };
  }
}
