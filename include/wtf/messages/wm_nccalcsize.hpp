/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{
    template <typename _super_t>
    struct wm_nccalcsize : _super_t{

      enum class activate_mode{
        active = WA_ACTIVE,
        click_active = WA_CLICKACTIVE,
        inactive = WA_INACTIVE,
      };
    protected:
      template <typename, template <typename> typename...> friend struct window_impl;

      virtual LRESULT on_wm_nccalcsize(NCCALCSIZE_PARAMS *) = 0;
      virtual LRESULT on_wm_nccalcsize(RECT *) = 0;

      explicit wm_nccalcsize(window * pParent)  : _super_t(pParent){}


      void handle_msg(wtf::window_message& msg) override {
        if (WM_NCCALCSIZE != msg.umsg) return;
        msg.bhandled = true;
        if (msg.wparam) msg.lresult = on_wm_nccalcsize(reinterpret_cast<NCCALCSIZE_PARAMS*>(msg.lparam));
        else  msg.lresult = on_wm_nccalcsize(reinterpret_cast<RECT*>(msg.lparam));
      }

    };
  }
}