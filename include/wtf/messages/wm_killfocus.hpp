/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{
    template <typename _super_t>
    struct wm_killfocus :  _super_t{

      callback<void(window * sender, HWND)> OnKillFocus;

      wm_killfocus() : _super_t() {}

    protected:
      template <typename, template <typename> typename...> friend struct window_impl;

      virtual void on_wm_killfocus(HWND hwnd){ OnKillFocus(this, hwnd); }

      void handle_msg(wtf::window_message& msg) override {
        if (WM_KILLFOCUS == msg.umsg) on_wm_killfocus(reinterpret_cast<HWND>(msg.wparam));
      }
    };
  }
}