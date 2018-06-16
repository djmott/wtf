/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {

  namespace policy {

    template <typename _super_t>
    struct wm_notifyformat : _super_t {

      wm_notifyformat() : _super_t() {}

    protected:

      template <typename, template <typename> typename...> friend struct window_impl;


      void handle_msg(wtf::window_message& msg) override {
        if (WM_NOTIFYFORMAT != msg.umsg) return;
#if defined(_UNICODE)
        msg.lresult = NFR_UNICODE;
#else
        msg.lresult = NFR_ANSI;
#endif
        msg.bhandled = true;
      }

    };
  }
}