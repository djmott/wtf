#pragma once
namespace wtf{

  enum event_vkeys{
    control = MK_CONTROL,
    left = MK_LBUTTON,
    middle = MK_MBUTTON,
    right = MK_RBUTTON,
    shift = MK_SHIFT,
    xbutton1 = MK_XBUTTON1,
    xbutton2 = MK_XBUTTON2,
  };

  template <typename _SuperT> struct wm_paint : _SuperT{
    wm_paint() = default;
    virtual ~wm_paint() = default;

    virtual void OnPaint(const device_context&, const rect&) {}
  protected:
    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      if (WM_PAINT == umsg){
        auto pPaint = reinterpret_cast<PAINTSTRUCT*>(lparam);
        auto r = reinterpret_cast<rect*>(wparam);
        device_context oCtx(pPaint->hdc);
        OnPaint(oCtx, *r);
      }
      return 0;
    }
  };


  template <typename _SuperT> struct wm_close : _SuperT{
    wm_close() = default;
    virtual ~wm_close() = default;
    
    virtual void OnClose(){}
  protected:
    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      if (WM_CLOSE == umsg) OnClose();
      return 0;
    }
  };

  template <typename _SuperT> struct wm_lbuttondown : _SuperT{
    wm_lbuttondown() = default;
    virtual ~wm_lbuttondown() = default;

    virtual void OnLMouseDown(event_vkeys, int x, int y){}

  protected:
    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      if (WM_LBUTTONDOWN == umsg) OnLMouseDown(static_cast<event_vkeys>(wparam), GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
      return 0;
    }

  };
  template <typename _SuperT> struct wm_lbuttonup : _SuperT{
    wm_lbuttonup() = default;
    virtual ~wm_lbuttonup() = default;

    virtual void OnLMouseUp(event_vkeys, int x, int y){}
  protected:

    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      if (WM_LBUTTONUP== umsg) OnLMouseUp(static_cast<event_vkeys>(wparam), GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
      return 0;
    }

  };

  template <typename _SuperT> struct wm_mousemove : _SuperT{
    wm_mousemove() = default;
    virtual ~wm_mousemove() = default;

    virtual void OnMouseMove(event_vkeys, int x, int y){}
  protected:

    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      if (WM_MOUSEMOVE == umsg) OnMouseMove(static_cast<event_vkeys>(wparam), LOWORD(lparam), HIWORD(lparam));
      return 0;
    }

  };

  enum class wm_size_flags{
    hide = SIZE_MAXHIDE,
    maximized = SIZE_MAXIMIZED,
    show = SIZE_MAXSHOW,
    minimized = SIZE_MINIMIZED,
    restored = SIZE_RESTORED,
  };

  template <typename _SuperT> struct wm_size : _SuperT{
    wm_size() = default;
    virtual ~wm_size() = default;


    virtual void OnResized(wm_size_flags, int width, int height) {}

  protected:
    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      if (WM_SIZE == umsg) OnResized(static_cast<wm_size_flags>(wparam), LOWORD(lparam), HIWORD(lparam));
      return 0;
    }
  };


  template <typename _SuperT> struct wm_move : _SuperT{
    wm_move() = default;
    virtual ~wm_move() = default;

    virtual void OnMoved(int x, int y){}

  protected:
    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      if (WM_MOVE == umsg) OnMoved(LOWORD(lparam), HIWORD(lparam));
      return 0;
    }
  };


  template <typename _SuperT> struct wm_moving : _SuperT{
    wm_moving() = default;
    virtual ~wm_moving() = default;

    virtual void OnMoving(LPRECT coords){}

  protected:
    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      if (WM_MOVING == umsg) OnMoving(reinterpret_cast<LPRECT>(lparam));
      return 0;
    }
  };


  enum class wm_nchittest_flags{
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

  template <typename _SuperT> struct wm_setcursor: _SuperT{
    wm_setcursor() = default;
    virtual ~wm_setcursor() = default;

    virtual void OnSetCursor(wm_nchittest_flags flags){}
  protected:
    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      if (WM_SETCURSOR == umsg) OnSetCursor(static_cast<wm_nchittest_flags>(LOWORD(lparam)));
      return 0;
    }
  };

}