/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{
    template <typename _SuperT>
    struct wm_setfocus : _SuperT{

    protected:

      virtual void on_wm_setfocus(HWND){}

      explicit wm_setfocus(window * pParent) : _SuperT(pParent){}

      void handle_msg(_::window_message& msg) override{
        if (WM_SETFOCUS == msg.umsg){
          on_wm_setfocus(reinterpret_cast<HWND>(msg.wparam));
        }
        _SuperT::handle_msg(msg);
      }
    };
  }
}
