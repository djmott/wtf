/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT>
    struct wm_destroy : _SuperT{

      callback<void(window * sender)> OnDestroy;

      void handle_msg(wtf::window_message& msg) override {
        if (WM_DESTROY == msg.umsg) on_wm_destroy();
      }

    protected:

      virtual void on_wm_destroy(){ OnDestroy(this); }

      explicit wm_destroy(window * pParent) noexcept : _SuperT(pParent){}

    };

  }
}