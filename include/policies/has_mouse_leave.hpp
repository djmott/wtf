#pragma once

namespace wtf{

  namespace policy{
    /** has_mouse_leave
    * produces mouse leave events
    */
    template<typename _SuperT, typename>
    struct has_mouse_leave : _SuperT{

      callback<void()> OnMouseLeave;

    protected:
      has_mouse_leave(iwindow * pParent) : _SuperT(pParent){}


      LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM, LPARAM, bool &) {
        if (WM_CREATE == umsg){
          TRACKMOUSEEVENT oEvent;
          memset(&oEvent, 0, sizeof(TRACKMOUSEEVENT));
          oEvent.cbSize = sizeof(TRACKMOUSEEVENT);
          oEvent.dwFlags = TME_LEAVE;
          oEvent.hwndTrack = hwnd;
          wtf::exception::throw_lasterr_if(::TrackMouseEvent(&oEvent), [](BOOL b){return !b; });
        } else if (WM_DESTROY == umsg){
          TRACKMOUSEEVENT oEvent;
          memset(&oEvent, 0, sizeof(TRACKMOUSEEVENT));
          oEvent.cbSize = sizeof(TRACKMOUSEEVENT);
          oEvent.dwFlags = TME_CANCEL | TME_LEAVE;
          oEvent.hwndTrack = hwnd;
          ::TrackMouseEvent(&oEvent);
        } else if (WM_MOUSELEAVE == umsg){
          OnMouseLeave();
        }
        return 0;
      }

    private:
    };
  }
}