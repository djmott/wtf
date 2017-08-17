/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{


  namespace policy{
    /** produces OnMouseLeave event 
    */
    template <typename _SuperT>
    struct wm_ncmouse_leave : _SuperT{

      callback<void(window * sender)> OnNCMouseLeave;


    protected:

      virtual void on_wm_ncmouse_leave(){ OnNCMouseLeave(this); }

      explicit wm_ncmouse_leave(window * pParent) : _SuperT(pParent){}

      void handle_msg(_::window_message& msg) override{
        if (WM_CREATE == msg.umsg){
          TRACKMOUSEEVENT oEvent;
          memset(&oEvent, 0, sizeof(TRACKMOUSEEVENT));
          oEvent.cbSize = sizeof(TRACKMOUSEEVENT);
          oEvent.dwFlags = TME_LEAVE;
          oEvent.hwndTrack = msg.hwnd;
          wtf::exception::throw_lasterr_if(::TrackMouseEvent(&oEvent), [](BOOL b){return !b; });
        } else if (WM_NCMOUSELEAVE == msg.umsg){
          on_wm_ncmouse_leave();
        }
        _SuperT::handle_msg(msg);
      }

    };
  }
}