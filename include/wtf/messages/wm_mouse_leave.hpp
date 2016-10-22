#pragma once

namespace wtf{

  namespace messages{
    /** has_mouse_leave
    * produces mouse leave events
    */
    template<typename _SuperT, typename>
    struct wm_mouse_leave : _SuperT{


    protected:

      virtual LRESULT on_wm_mouse_leave(bool& bHandled) = 0;

      wm_mouse_leave(window<void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM, LPARAM, bool & bHandled) {
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
          return on_wm_mouse_leave(bHandled);
        }
        return 0;
      }

    private:
    };
  }
}