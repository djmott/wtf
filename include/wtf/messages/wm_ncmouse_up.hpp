/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT>
    struct wm_ncmouse_up : _SuperT{

      callback<void(window *, const mouse_msg_param<coord_frame::screen>&)> OnNCMouseUp;

    protected:
      virtual void on_wm_ncmouse_up(const mouse_msg_param<coord_frame::screen>& param){ OnNCMouseUp(this, param); }

      explicit wm_ncmouse_up(window * pParent) : _SuperT(pParent){}

      void handle_msg(_::window_message& msg) override{
        if (WM_NCLBUTTONUP == msg.umsg) on_wm_ncmouse_up(mouse_msg_param<coord_frame::screen>(msg.lparam, mouse_buttons::left));
        else if (WM_NCMBUTTONUP == msg.umsg) on_wm_ncmouse_up(mouse_msg_param<coord_frame::screen>(msg.lparam, mouse_buttons::middle));
        else if (WM_NCRBUTTONUP == msg.umsg) on_wm_ncmouse_up(mouse_msg_param<coord_frame::screen>(msg.lparam, mouse_buttons::right));
        _SuperT::handle_msg(msg);
      }
    };
  }
}