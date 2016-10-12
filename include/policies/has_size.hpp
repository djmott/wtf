#pragma once

namespace wtf {
  namespace policy {
    /** has_size
    * Adds members to reposition/resize and events to capture them
    */
    template<typename _SuperT, typename _ImplT>
    struct has_size : _SuperT {

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

      void poke() {
        wtf::exception::throw_lasterr_if(::SetWindowPos(*this, HWND_TOP, 0, 0, 0, 0, 
                                         SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW |
                                         SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER | 
                                         SWP_NOSENDCHANGING | SWP_NOREPOSITION
                                         ),
                                         [](BOOL b){ return !b; });
      }

      virtual int top() const {
        auto rc = rect::client_coord::get(*this);
        auto hParent = wtf::exception::throw_lasterr_if(::GetParent(*this), [](HWND h){ return !h; });
        SetLastError(ERROR_INVALID_PARAMETER);
        MapWindowPoints( *this, hParent, reinterpret_cast<LPPOINT>(&rc), 2);
        return rc.top; 
      }

      virtual int left() const { 
        auto rc = rect::client_coord::get(*this);
        auto hParent = wtf::exception::throw_lasterr_if(::GetParent(*this), [](HWND h){ return !h; });
        MapWindowPoints( *this, hParent, reinterpret_cast<LPPOINT>(&rc), 2);
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

      callback<void(rect::screen_coords&)> OnMoving;
      callback<void(const point::client_coords&)> OnMoved;
      callback<void(rect::screen_coords&)> OnResizing;
      callback<void(const point::client_coords&)> OnResized;

    protected:

      virtual void MovingEvent(rect::screen_coords& r){ OnMoving(r); }
      virtual void MovedEvent(const point::client_coords& r){ OnMoved(r); }
      virtual void ResizingEvent(rect::screen_coords& r){ OnResizing(r); }
      virtual void ResizedEvent(wm_size_flags, const point::client_coords& r){ OnResized(r); }


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