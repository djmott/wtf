/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{
    template <typename _SuperT>
    struct wm_timer : _SuperT{

      callback<void(window * sender, UINT_PTR timer_id)> OnTimer;

    protected:
      explicit wm_timer(window * pParent) : _SuperT(pParent){}

      virtual void on_wm_timer(UINT_PTR timer_id){ OnTimer(this, timer_id); }

      void handle_msg(_::window_message& msg) override{
        if (WM_TIMER == msg.umsg) on_wm_timer(static_cast<UINT_PTR>(msg.wparam));
        _SuperT::handle_msg(msg);
      }

    };

  }
}