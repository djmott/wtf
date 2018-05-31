/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{
    template <typename _SuperT>
    struct wm_setfocus : _SuperT{

      callback<void(window * sender, HWND)> OnSetFocus;

      void handle_msg(wtf::window_message& msg) override {
        if (WM_SETFOCUS == msg.umsg) {
          on_wm_setfocus(reinterpret_cast<HWND>(msg.wparam));
        }

      }
    protected:

      virtual void on_wm_setfocus(HWND hwnd){ OnSetFocus(this, hwnd); }

      explicit wm_setfocus(window * pParent) : _SuperT(pParent){}

    };
  }
}
