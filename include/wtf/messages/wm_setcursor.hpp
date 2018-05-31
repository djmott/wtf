/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT>
    struct wm_setcursor : _SuperT{

      void handle_msg(wtf::window_message& msg) override {
        if (WM_SETCURSOR == msg.umsg) {
          on_wm_setcursor(static_cast<wm_nchittest_flags>(LOWORD(msg.lparam)));
          msg.lresult = TRUE;
          msg.bhandled = true;
        }

      }
    protected:

      virtual void on_wm_setcursor(wm_nchittest_flags)  {}

      explicit wm_setcursor(window * pParent) noexcept : _SuperT(pParent){}

    };
  }
}