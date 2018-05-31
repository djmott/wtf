/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT>
    struct wm_mouse_wheel : _SuperT{

      callback<void(window * sender, int16_t delta, const mouse_msg_param<coord_frame::client>& param)> OnMouseWheel;

      void handle_msg(wtf::window_message& msg) override {
        if (WM_MOUSEWHEEL != msg.umsg) return
          on_wm_mouse_wheel(static_cast<int16_t>(HIWORD(msg.wparam)), mouse_msg_param<coord_frame::client>(msg.wparam, msg.lparam, mouse_buttons::unspecified));
      }

    protected:

      virtual void on_wm_mouse_wheel(int16_t delta, const mouse_msg_param<coord_frame::client>& param){ OnMouseWheel(this, delta, param); }

      explicit wm_mouse_wheel(window * pParent) : _SuperT(pParent){}

    };
  }
}