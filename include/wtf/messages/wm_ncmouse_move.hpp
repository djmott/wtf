/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT>
    struct wm_ncmouse_move : _SuperT{

      callback<void(window * sender, const mouse_msg_param<coord_frame::screen>&)> OnNCMouseMove;

    protected:

      virtual void on_wm_ncmouse_move(const mouse_msg_param<coord_frame::screen>& param){ OnNCMouseMove(this, param); }

      explicit wm_ncmouse_move(window * pParent) : _SuperT(pParent){}

      void handle_msg(_::window_message& msg) override{
        if (WM_NCMOUSEMOVE == msg.umsg){
          on_wm_ncmouse_move(mouse_msg_param<coord_frame::screen>(msg.lparam, mouse_buttons::unspecified));
        }
        _SuperT::handle_msg(msg);
      }
    };
  }
}