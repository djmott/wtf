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

    virtual void OnPaint(const RECT& area, Gdiplus::Graphics&){}

    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wpara, LPARAM lparam, bool& bhandled) override{
      RECT update_rect;
      if (WM_PAINT == umsg && GetUpdateRect(hwnd, &update_rect, FALSE)){
        PAINTSTRUCT paint;
        BeginPaint(hwnd, &paint);
          
        OnPaint(update_rect, Gdiplus::Graphics(paint.hdc));
        EndPaint(hwnd, &paint);
      }
      return 0;
    }
  private:
  };


  template <typename _SuperT> struct wm_close : _SuperT{
    wm_close() = default;
    virtual ~wm_close() = default;
    
    virtual void OnClose(){}
    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wpara, LPARAM lparam, bool& bhandled) override{
      if (WM_CLOSE == umsg) OnClose();
      return 0;
    }
  };

  template <typename _SuperT> struct wm_lbuttondown : _SuperT{
    wm_lbuttondown() = default;
    virtual ~wm_lbuttondown() = default;

    virtual void OnLMouseDown(event_vkeys, int x, int y){}

    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wpara, LPARAM lparam, bool& bhandled) override{
      if (WM_LBUTTONDOWN == umsg) OnLMouseDown(static_cast<event_vkeys>(wpara), GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
      return 0;
    }

  };
  template <typename _SuperT> struct wm_lbuttonup : _SuperT{
    wm_lbuttonup() = default;
    virtual ~wm_lbuttonup() = default;

    virtual void OnLMouseUp(event_vkeys, int x, int y){}

    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wpara, LPARAM lparam, bool& bhandled) override{
      if (WM_LBUTTONUP== umsg) OnLMouseUp(static_cast<event_vkeys>(wpara), GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
      return 0;
    }

  };

  template <typename _SuperT> struct wm_mousemove : _SuperT{
    wm_mousemove() = default;
    virtual ~wm_mousemove() = default;

    virtual void OnMouseMove(event_vkeys, int x, int y){}

    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wpara, LPARAM lparam, bool& bhandled) override{
      if (WM_MOUSEMOVE == umsg) OnMouseMove(static_cast<event_vkeys>(wpara), LOWORD(lparam), HIWORD(lparam));
      return 0;
    }

  };

  template <typename _SuperT> struct wm_size : _SuperT{
    wm_size() = default;
    virtual ~wm_size() = default;

    enum class type{
      hide = SIZE_MAXHIDE,
      maximized = SIZE_MAXIMIZED,
      show = SIZE_MAXSHOW,
      minimized = SIZE_MINIMIZED,
      restored = SIZE_RESTORED,
    };

    virtual void OnResized(type, int width, int height){}

    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wpara, LPARAM lparam, bool& bhandled) override{
      if (WM_SIZE == umsg) OnResized(static_cast<type>(wpara), LOWORD(lparam), HIWORD(lparam));
      return 0;
    }
  };


  template <typename _SuperT> struct wm_move : _SuperT{
    wm_move() = default;
    virtual ~wm_move() = default;

    virtual void OnMoved(int x, int y){}

    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wpara, LPARAM lparam, bool& bhandled) override{
      if (WM_MOVE == umsg) OnMoved(LOWORD(lparam), HIWORD(lparam));
      return 0;
    }
  };


  template <typename _SuperT> struct wm_moving : _SuperT{
    wm_moving() = default;
    virtual ~wm_moving() = default;

    virtual void OnMoving(LPRECT coords){}

    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wpara, LPARAM lparam, bool& bhandled) override{
      if (WM_MOVING == umsg) OnMoving(reinterpret_cast<LPRECT>(lparam));
      return 0;
    }
  };

}