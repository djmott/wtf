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

    protected:
      virtual void OnMoving(rect::screen_coords&) {}
      virtual void OnMoved(const point::client_coords&) {}
      virtual void OnResizing(rect::screen_coords&) {}
      virtual void OnResized(const point::client_coords&) {}


      has_size(window<void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND , UINT umsg, WPARAM wparam, LPARAM lparam, bool & bHandled) {
        LRESULT lret = 0;
        bHandled = true;
        if (WM_MOVING == umsg){
          OnMoving(*reinterpret_cast<rect::screen_coords*>(lparam));
          lret = TRUE;
        } else if (WM_MOVE == umsg){
          OnMoved(point::client_coords(LOWORD(lparam), HIWORD(lparam)));
        } else if (WM_SIZING == umsg){
          OnResizing(*reinterpret_cast<rect::screen_coords*>(lparam));
          lret = TRUE;
        } else if (WM_SIZE == umsg){
          OnResized( point::client_coords(LOWORD(lparam), HIWORD(lparam)));
        } else{
          bHandled = false;
        }
        return lret;
      }
    private:

    };

  }
}
