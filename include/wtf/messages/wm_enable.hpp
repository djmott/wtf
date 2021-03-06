﻿/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace messages {

    template <typename _super_t>
    struct wm_enable : _super_t{

      callback<void(window * sender)> OnEnable;
      callback<void(window * sender)> OnDisable;

    protected:
      template <typename, template <typename> typename...> friend struct window_impl;

      virtual void on_wm_enable(bool bEnable){ 
        if (bEnable) OnEnable(this);
        else OnDisable(this);
      }


      void handle_msg(wtf::window_message& msg) override {
        if (WM_ENABLE != msg.umsg) return;
        if (msg.wparam) on_wm_enable(true);
        else on_wm_enable(false);
      }


    };

  }
}