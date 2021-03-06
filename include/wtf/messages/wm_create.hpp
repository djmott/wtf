﻿/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace messages {
    template <typename _super_t>
    struct wm_create : _super_t{

      callback<void(window * sender)> OnCreate;

    protected:
      template <typename, template <typename> typename...> friend struct window_impl;

      virtual void on_wm_create(){ OnCreate(this); }


      void handle_msg(wtf::window_message& msg) override {
        if (WM_CREATE == msg.umsg) on_wm_create();
      }

    };

  }
}