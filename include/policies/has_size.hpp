#pragma once

namespace wtf {
  namespace policy {
    /** has_size
    * Adds members to reposition/resize and events to capture them
    */
    template<typename _SuperT, typename _ImplT>
    struct has_size : _SuperT {
      has_size(): _SuperT(){}
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
        wtf::exception::throw_lasterr_if(::MoveWindow(*this, x, y, width, height, repaint ? TRUE : FALSE),
                                         [](BOOL b) { return !b; });
      }


      virtual int top() const {
        auto rc = rect::client_coord::get(*this);
        auto hParent = wtf::exception::throw_lasterr_if(::GetParent(*this), [](HWND h){ return !h; });
        SetLastError(ERROR_INVALID_PARAMETER);
        auto iRet = MapWindowPoints( *this, hParent, reinterpret_cast<LPPOINT>(&rc), 2);
//         wtf::exception::throw_lasterr_if(iRet, [](int i){ return i == ERROR_INVALID_PARAMETER; });
        return rc.top; 
      }

      virtual int left() const { 
        auto rc = rect::client_coord::get(*this);
        auto hParent = wtf::exception::throw_lasterr_if(::GetParent(*this), [](HWND h){ return !h; });
        auto iRet = MapWindowPoints( *this, hParent, reinterpret_cast<LPPOINT>(&rc), 2);
//         wtf::exception::throw_lasterr_if(iRet, [](int i){ return !i; });
        return rc.left;
      }

      virtual int width() const{ 
        auto r = rect::client_coord::get(*this);
        return r.right - r.left ; 
      }

      virtual int height() const{
        auto r = rect::client_coord::get(*this);
        return r.bottom - r.top;
      }


    protected:

      virtual void MovingEvent(rect::screen_coords&){}
      virtual void MovedEvent(const point::client_coords&){}
      virtual void ResizingEvent(rect::screen_coords&){}
      virtual void ResizedEvent(wm_size_flags, const point::client_coords&){}


      virtual LRESULT handle_message(HWND , UINT umsg, WPARAM wparam, LPARAM lparam, bool & bHandled) override {
        LRESULT lret = 0;
        bHandled = true;
        if (WM_MOVING == umsg){
          MovingEvent(*reinterpret_cast<rect::screen_coords*>(lparam));
          lret = TRUE;
        } else if (WM_MOVE == umsg){
          MovedEvent(point::client_coords(LOWORD(lparam), HIWORD(lparam)));
        } else if (WM_SIZING == umsg){
          ResizingEvent(*reinterpret_cast<rect::screen_coords*>(lparam));
          lret = TRUE;
        } else if (WM_SIZE == umsg){
          ResizedEvent(static_cast<wm_size_flags>(wparam), point::client_coords(LOWORD(lparam), HIWORD(lparam)));
        } else{
          bHandled = false;
        }
        return lret;
      }
    private:

    };

  }
}
