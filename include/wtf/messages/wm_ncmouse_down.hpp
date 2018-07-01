/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once


namespace wtf{
  namespace messages {

    template <typename _super_t>
    struct wm_ncmouse_down : _super_t{

      callback<void(window * sender, const mouse_msg_param<coord_frame::screen>&)> OnNCMouseDown;

    protected:
      template <typename, template <typename> typename...> friend struct window_impl;

      virtual void on_wm_ncmouse_down(const mouse_msg_param<coord_frame::screen>& param){ OnNCMouseDown(this, param); }


      void handle_msg(wtf::window_message& msg) override {
        if (WM_NCLBUTTONDOWN == msg.umsg) on_wm_ncmouse_down(mouse_msg_param<coord_frame::screen>(msg.lparam, mouse_buttons::left));
        else if (WM_NCMBUTTONDOWN == msg.umsg) on_wm_ncmouse_down(mouse_msg_param<coord_frame::screen>(msg.lparam, mouse_buttons::middle));
        else if (WM_NCRBUTTONDOWN == msg.umsg) on_wm_ncmouse_down(mouse_msg_param<coord_frame::screen>(msg.lparam, mouse_buttons::right));
      }

    };
  }
}