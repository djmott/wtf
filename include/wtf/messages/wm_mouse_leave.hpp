#pragma once

namespace wtf{

  /** has_mouse_leave
  * produces mouse leave events
  */

  namespace policy{
    template <typename _ImplT, typename _SuperT>
    class wm_mouse_leave : public _SuperT{

    protected:

      virtual void on_wm_mouse_leave(){}

      explicit wm_mouse_leave(iwindow * pParent) : _SuperT(pParent){}

      virtual void handle_msg(window_message& msg) override{
        if (WM_CREATE == msg.umsg){
          TRACKMOUSEEVENT oEvent;
          memset(&oEvent, 0, sizeof(TRACKMOUSEEVENT));
          oEvent.cbSize = sizeof(TRACKMOUSEEVENT);
          oEvent.dwFlags = TME_LEAVE;
          oEvent.hwndTrack = hwnd;
          wtf::exception::throw_lasterr_if(::TrackMouseEvent(&oEvent), [](BOOL b){return !b; });
        } else if (WM_MOUSELEAVE == msg.umsg){
          on_wm_mouse_leave();
        }
      }

    };
  }
}
