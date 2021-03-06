﻿/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#define DOXY_INHERIT_WM_DBLCLICK

namespace wtf{
  namespace messages {
    template <typename _super_t>
    struct wm_dblclick : _super_t{

      callback<void(window * sender, const mouse_msg_param<coord_frame::client>&)> OnDblClick;
      

    protected:

      template <typename, template <typename> typename...> friend struct window_impl;

      virtual void on_wm_dblclick(const mouse_msg_param<coord_frame::client>& p){ OnDblClick(this, p); }

      void handle_msg(wtf::window_message& msg) override {
        if (WM_LBUTTONDBLCLK == msg.umsg) {
          on_wm_dblclick(mouse_msg_param<coord_frame::client>(msg.wparam, msg.lparam, mouse_buttons::left));
        }
        else if (WM_MBUTTONDBLCLK == msg.umsg) {
          on_wm_dblclick(mouse_msg_param<coord_frame::client>(msg.wparam, msg.lparam, mouse_buttons::middle));
        }
        else if (WM_RBUTTONDBLCLK == msg.umsg) {
          on_wm_dblclick(mouse_msg_param<coord_frame::client>(msg.wparam, msg.lparam, mouse_buttons::right));
        }
      }

    };
  }
}
