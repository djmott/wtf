#pragma once

namespace wtf {
  namespace policy {
    /** has_size
    * Adds members to reposition/resize and events to capture them
    */
    template<typename _SuperT>
    struct has_size : _SuperT {
      enum class wm_size_flags {
        hide = SIZE_MAXHIDE,
        maximized = SIZE_MAXIMIZED,
        show = SIZE_MAXSHOW,
        minimized = SIZE_MINIMIZED,
        restored = SIZE_RESTORED,
      };

      void move(int x, int y, int width, int height, bool repaint = true) {
        _left = x;
        _top = y;
        _width = width;
        _height = height;
        wtf::exception::throw_lasterr_if(::MoveWindow(*this, x, y, width, height, repaint ? TRUE : FALSE),
                                         [](BOOL b) { return !b; });
      }

      int top() const { return _top; }

      int left() const { return _left; }

      int width() const { return _width; }

      int height() const { return _height; }

      has_size()
        : _SuperT(), _top(CW_USEDEFAULT), _left(CW_USEDEFAULT), _width(CW_USEDEFAULT), _height(CW_USEDEFAULT) {}

      callback<void(
      const rect&)>
      MovingEvent;
      callback<void(
      const point&)>
      MovedEvent;
      callback<void(wm_size_flags, int /*width*/, int /*height*/)> ResizedEvent;

    protected:
      int _top, _left, _width, _height;

      virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool &bhandled) override {
        if (WM_MOVING == umsg) MovingEvent(*reinterpret_cast<const rect *>(lparam));
        if (WM_MOVE == umsg) MovedEvent(point(LOWORD(lparam), HIWORD(lparam)));
        if (WM_SIZE == umsg) ResizedEvent(static_cast<wm_size_flags>(wparam), LOWORD(lparam), HIWORD(lparam));
        return 0;
      }
    };

  }
}
