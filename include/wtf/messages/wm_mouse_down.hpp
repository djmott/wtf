/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once


namespace wtf{

  namespace policy{
    template <typename _SuperT>
    struct wm_mouse_down : _SuperT{

      callback<void(window * sender, const mouse_msg_param<coord_frame::client>&)> OnMouseDown;

      void handle_msg(wtf::window_message& msg) override {
        if (WM_LBUTTONDOWN == msg.umsg) on_wm_mouse_down(mouse_msg_param<coord_frame::client>(msg.wparam, msg.lparam, mouse_buttons::left));
        else if (WM_MBUTTONDOWN == msg.umsg) on_wm_mouse_down(mouse_msg_param<coord_frame::client>(msg.wparam, msg.lparam, mouse_buttons::middle));
        else if (WM_RBUTTONDOWN == msg.umsg) on_wm_mouse_down(mouse_msg_param<coord_frame::client>(msg.wparam, msg.lparam, mouse_buttons::right));
      }

    protected:

      virtual void on_wm_mouse_down(const mouse_msg_param<coord_frame::client>& param){ OnMouseDown(this, param); }

      explicit wm_mouse_down(window * pParent) : _SuperT(pParent){}

    };
  }
}