/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{
    template <typename _SuperT>
    struct wm_close : _SuperT{

      callback<void(window * sender, bool& cancel)> OnClose;

      void handle_msg(wtf::window_message& msg) override {
        bool cancel = false;
        if (WM_CLOSE != msg.umsg) return;
        on_wm_close(cancel);
        if (cancel) msg.bhandled = true;
      }

    protected:

      virtual void on_wm_close(bool& cancel){ OnClose(this, cancel); }

      explicit wm_close(window * pParent) : _SuperT(pParent){}


    };

  }
}