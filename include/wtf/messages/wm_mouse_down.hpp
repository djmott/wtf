﻿/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#define DOXY_INHERIT_WM_MOUSE_DOWN

namespace wtf{
  namespace messages {

    template <typename _super_t>
    struct wm_mouse_down : _super_t{

      callback<void(window * sender, const mouse_msg_param<coord_frame::client>&)> OnMouseDown;

    protected:
      template <typename, template <typename> typename...> friend struct window_impl;

      virtual void on_wm_mouse_down(const mouse_msg_param<coord_frame::client>& param){ OnMouseDown(this, param); }

      void handle_msg(wtf::window_message& msg) override {
        if (WM_LBUTTONDOWN == msg.umsg) on_wm_mouse_down(mouse_msg_param<coord_frame::client>(msg.wparam, msg.lparam, mouse_buttons::left));
        else if (WM_MBUTTONDOWN == msg.umsg) on_wm_mouse_down(mouse_msg_param<coord_frame::client>(msg.wparam, msg.lparam, mouse_buttons::middle));
        else if (WM_RBUTTONDOWN == msg.umsg) on_wm_mouse_down(mouse_msg_param<coord_frame::client>(msg.wparam, msg.lparam, mouse_buttons::right));
      }
    
    };
  }
}