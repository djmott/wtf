/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{
    template <typename _super_t>
    struct wm_close : _super_t{

      callback<void(window * sender, bool& cancel)> OnClose;

    protected:
      template <typename, template <typename> typename...> friend struct window_impl;

      virtual void on_wm_close(bool& cancel){ OnClose(this, cancel); }

      explicit wm_close(window * pParent) : _super_t(pParent){}

      void handle_msg(wtf::window_message& msg) override {
        bool cancel = false;
        if (WM_CLOSE != msg.umsg) return;
        on_wm_close(cancel);
        if (cancel) msg.bhandled = true;
      }



    };

  }
}