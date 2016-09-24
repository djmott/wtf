#pragma once
namespace wtf{

  template <typename _SuperT> struct wm_mousemove : _SuperT{

    enum class buttons{
      control = MK_CONTROL,
      left = MK_LBUTTON,
      middle = MK_MBUTTON,
      right = MK_RBUTTON,
      shift = MK_SHIFT,
      xbutton1 = MK_XBUTTON1,
      xbutton2 = MK_XBUTTON2,
    };

    virtual void OnMouseMove(buttons, int x, int y){}

    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wpara, LPARAM lparam, bool& bhandled) override{
      if (WM_SIZE == umsg) OnMouseMove(static_cast<buttons>(wpara), LOWORD(lparam), HIWORD(lparam));
      return 0;
    }

  };

  template <typename _SuperT> struct wm_size : _SuperT{

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

    virtual void OnMoved(int x, int y){}

    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wpara, LPARAM lparam, bool& bhandled) override{
      if (WM_MOVE == umsg) OnMoved(LOWORD(lparam), HIWORD(lparam));
      return 0;
    }
  };


  template <typename _SuperT> struct wm_moving : _SuperT{

    virtual void OnMoving(LPRECT coords){}

    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wpara, LPARAM lparam, bool& bhandled) override{
      if (WM_MOVING == umsg) OnMoving(reinterpret_cast<LPRECT>(lparam));
      return 0;
    }
  };

}