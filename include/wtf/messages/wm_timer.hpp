/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{
    template <typename _super_t> struct wm_timer : _super_t{

      callback<void(window * sender, UINT_PTR timer_id)> OnTimer;

      wm_timer() : _super_t() {}

    protected:
      template <typename, template <typename> typename...> friend struct window_impl;

      virtual void on_wm_timer(UINT_PTR timer_id){ OnTimer(this, timer_id); }

      void handle_msg(wtf::window_message& msg) override {
        if (WM_TIMER == msg.umsg) on_wm_timer(static_cast<UINT_PTR>(msg.wparam));
      }
    };
  }
}