/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{
    template <typename _SuperT>
    struct wm_close : _SuperT{

      callback<void()> OnClose;

    protected:

      void handle_msg(_::window_message& msg) override{
        if (WM_CLOSE == msg.umsg) on_wm_close();
        _SuperT::handle_msg(msg);
      }


      virtual void on_wm_close(){ OnClose(); }

      explicit wm_close(iwindow * pParent) : _SuperT(pParent){}


    };

  }
}