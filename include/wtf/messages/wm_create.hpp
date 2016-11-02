/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{
    template <typename _SuperT>
    struct wm_create : _SuperT{

      callback<void()> OnCreate;

    protected:

      virtual void on_wm_create(){ OnCreate(); }

      explicit wm_create(iwindow * pParent) : _SuperT(pParent){}

      void handle_msg(_::window_message& msg) override{
        if (WM_CREATE == msg.umsg) on_wm_create();
        _SuperT::handle_msg(msg);
      }

    };

  }
}