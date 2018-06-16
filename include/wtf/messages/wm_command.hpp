/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace policy {

    template <typename _super_t>
    struct wm_command : _super_t {

      callback<void(window * sender, WPARAM, LPARAM)> OnCommand;

      wm_command() : _super_t() {}

    protected:
      template <typename, template <typename> typename...> friend struct window_impl;

      virtual void on_wm_command(WPARAM wparam,LPARAM lparam) { OnCommand(this, wparam, lparam); }


      void handle_msg(wtf::window_message& msg) override {
        if (WM_COMMAND != msg.umsg) return;
        on_wm_command(msg.wparam, msg.lparam);        
      }

    };

  }
}