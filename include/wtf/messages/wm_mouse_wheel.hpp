/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT>
    struct wm_mouse_wheel : _SuperT{

      callback<void(window * sender, int16_t delta, const mouse_msg_param& param)> OnMouseWheel;

    protected:

      virtual void on_wm_mouse_wheel(int16_t delta, const mouse_msg_param& param){ OnMouseWheel(this, delta, param); }

      explicit wm_mouse_wheel(window * pParent) : _SuperT(pParent){}

      void handle_msg(_::window_message& msg) override{
        if (WM_MOUSEWHEEL == msg.umsg){
          on_wm_mouse_wheel(static_cast<int16_t>(HIWORD(msg.wparam)), mouse_msg_param(msg.wparam, msg.lparam, mouse_msg_param::buttons::unspecified));
        }
        _SuperT::handle_msg(msg);
      }
    };
  }
}