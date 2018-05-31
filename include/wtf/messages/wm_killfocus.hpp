/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT>
    struct wm_killfocus :  _SuperT{

      callback<void(window * sender, HWND)> OnKillFocus;

      void handle_msg(wtf::window_message& msg) override {
        if (WM_KILLFOCUS == msg.umsg) on_wm_killfocus(reinterpret_cast<HWND>(msg.wparam));
      }

    protected:

      virtual void on_wm_killfocus(HWND hwnd){ OnKillFocus(this, hwnd); }

      explicit wm_killfocus(window * pParent) noexcept : _SuperT(pParent){}
    };
  }
}