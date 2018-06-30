/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#define DOXY_INHERIT_WM_NOTIFY

namespace wtf {

  namespace policy {

    template <typename _super_t>
    struct wm_notify : _super_t {

    protected:

      template <typename, template <typename> typename...> friend struct window_impl;

      virtual void on_wm_notify(NMHDR * notification) { }

      void handle_msg(wtf::window_message& msg) override {
        if (WM_NOTIFY != msg.umsg) return;
        on_wm_notify(reinterpret_cast<NMHDR*>(msg.lparam));
      }

    };
  }
}