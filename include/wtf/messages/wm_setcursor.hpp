﻿/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace messages {

    template <typename _super_t>
    struct wm_setcursor : _super_t{

    protected:

      template <typename, template <typename> typename...> friend struct window_impl;
      
      virtual void on_wm_setcursor(wm_nchittest_flags) {}

      void handle_msg(wtf::window_message& msg) override {
        if (WM_SETCURSOR == msg.umsg) {
          on_wm_setcursor(static_cast<wm_nchittest_flags>(LOWORD(msg.lparam)));
          msg.lresult = TRUE;
          msg.bhandled = true;
        }

      }
    };
  }
}