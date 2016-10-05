#pragma once

namespace wtf {
  namespace policy {
    /** has_size
    * Adds members to reposition/resize and events to capture them
    */
    template<typename _SuperT>
    struct has_size : _SuperT {
      has_size()
        : _SuperT(), _top(CW_USEDEFAULT), _left(CW_USEDEFAULT), _width(CW_USEDEFAULT), _height(CW_USEDEFAULT){}
      virtual ~has_size() = default;
      has_size(const has_size&) = delete;
      has_size &operator=(const has_size &) = delete;
      has_size(has_size&&) = delete;
      has_size &operator=(has_size&&) = delete;

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


    protected:

      virtual void MovingEvent(const rect&){}
      virtual void MovedEvent(const point&){}
      virtual void ResizedEvent(wm_size_flags, uint16_t /*width*/, uint16_t /*height*/){}


      virtual LRESULT handle_message(HWND , UINT umsg, WPARAM wparam, LPARAM lparam, bool &) override {
        if (WM_MOVING == umsg) MovingEvent(*reinterpret_cast<const rect *>(lparam));
        if (WM_MOVE == umsg) MovedEvent(point(LOWORD(lparam), HIWORD(lparam)));
        if (WM_SIZE == umsg) ResizedEvent(static_cast<wm_size_flags>(wparam), LOWORD(lparam), HIWORD(lparam));
        return 0;
      }
      int _top, _left, _width, _height;
    };

  }
}
