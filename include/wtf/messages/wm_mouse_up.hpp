/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{
    template <typename _super_t>
    struct wm_mouse_up : _super_t{

      callback<void(window *, const mouse_msg_param<coord_frame::client>&)> OnMouseUp;

    protected:
      template <typename, template <typename> typename...> friend struct window_impl;

      virtual void on_wm_mouse_up(const mouse_msg_param<coord_frame::client>& param){ OnMouseUp(this, param); }


      void handle_msg(wtf::window_message& msg) override {
        if (WM_LBUTTONUP == msg.umsg) on_wm_mouse_up(mouse_msg_param<coord_frame::client>(msg.wparam, msg.lparam, mouse_buttons::left));
        else if (WM_MBUTTONUP == msg.umsg) on_wm_mouse_up(mouse_msg_param<coord_frame::client>(msg.wparam, msg.lparam, mouse_buttons::middle));
        else if (WM_RBUTTONUP == msg.umsg) on_wm_mouse_up(mouse_msg_param<coord_frame::client>(msg.wparam, msg.lparam, mouse_buttons::right));
      }

    };
  }
}