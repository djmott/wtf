/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{
    template <typename _super_t>
    struct wm_mouse_move : _super_t{

      callback<void(window * sender, const mouse_msg_param<coord_frame::client>&)> OnMouseMove;

    protected:
      template <typename, template <typename> typename...> friend struct window_impl;

      virtual void on_wm_mouse_move(const mouse_msg_param<coord_frame::client>& param){ OnMouseMove(this, param); }

      void handle_msg(wtf::window_message& msg) override {
        if (WM_MOUSEMOVE == msg.umsg) {
          on_wm_mouse_move(mouse_msg_param<coord_frame::client>(msg.wparam, msg.lparam, mouse_buttons::unspecified));
        }
      }

    };
  }
}