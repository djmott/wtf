/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{

    template <typename _SuperT>
    struct wm_enable : _SuperT{

      callback<void(window * sender)> OnEnable;
      callback<void(window * sender)> OnDisable;

    protected:

      virtual void on_wm_enable(bool bEnable){ 
        if (bEnable) OnEnable(this);
        else OnDisable(this);
      }

      explicit wm_enable(window * pParent) : _SuperT(pParent){}

      void handle_msg(_::window_message& msg) override{
        if (WM_ENABLE == msg.umsg){
          if (msg.wparam) on_wm_enable(true);
          else on_wm_enable(false);
        }
        _SuperT::handle_msg(msg);
      }

    };

  }
}