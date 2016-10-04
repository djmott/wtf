#pragma once

namespace wtf {

  enum class event_vkeys{
    control = MK_CONTROL,
    left = MK_LBUTTON,
    middle = MK_MBUTTON,
    right = MK_RBUTTON,
    shift = MK_SHIFT,
    xbutton1 = MK_XBUTTON1,
    xbutton2 = MK_XBUTTON2,
  };

  namespace policy {
    /** has_mouse
    * produces events generated by mouse actions
    */
    template<typename _SuperT>
    struct has_mouse : _SuperT {


      callback<void(event_vkeys, const point &)> MouseMoveEvent;
      callback<void(event_vkeys, const point &)> MouseLButtonDownEvent;
      callback<void(event_vkeys, const point &)> MouseLButtonUpEvent;
      callback<void(event_vkeys, short delta, const point &)> MouseWheelEvent;
    protected:
      virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool &bhandled) override {
        if (WM_MOUSEMOVE == umsg)
          MouseMoveEvent(static_cast<event_vkeys>(wparam), point(LOWORD(lparam), HIWORD(lparam)));
        if (WM_LBUTTONDOWN == umsg)
          MouseLButtonDownEvent(static_cast<event_vkeys>(wparam), point(LOWORD(lparam), HIWORD(lparam)));
        if (WM_LBUTTONUP == umsg)
          MouseLButtonUpEvent(static_cast<event_vkeys>(wparam), point(LOWORD(lparam), HIWORD(lparam)));
        if (WM_MOUSEWHEEL == umsg)
          MouseWheelEvent(static_cast<event_vkeys>(LOWORD(wparam)), static_cast<short>(HIWORD(wparam)),
                          point(LOWORD(lparam), HIWORD(lparam)));
        return 0;
      }
    };
  }
}
