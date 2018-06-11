/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{


  namespace policy{
    /** produces OnMouseLeave event 
    */
    template <typename _super_t>
    struct wm_mouse_leave : _super_t{

      callback<void(window * sender)> OnMouseLeave;


    protected:
      template <typename, template <typename> typename...> friend struct window_impl;

      virtual void on_wm_mouse_leave(){ OnMouseLeave(this); }

      explicit wm_mouse_leave(window * pParent)  : _super_t(pParent){}

      void handle_msg(wtf::window_message& msg)  override {
        if (WM_CREATE == msg.umsg) {
          TRACKMOUSEEVENT oEvent;
          memset(&oEvent, 0, sizeof(TRACKMOUSEEVENT));
          oEvent.cbSize = sizeof(TRACKMOUSEEVENT);
          oEvent.dwFlags = TME_LEAVE;
          oEvent.hwndTrack = msg.hwnd;
          wtf::exception::throw_lasterr_if(::TrackMouseEvent(&oEvent), [](BOOL b) noexcept {return !b; });
        }
        else if (WM_MOUSELEAVE == msg.umsg) {
          on_wm_mouse_leave();
        }
      }

    };
  }
}
