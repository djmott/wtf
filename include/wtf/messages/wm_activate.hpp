/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{

    template <typename _SuperT>
    struct wm_activate : _SuperT{

      enum class activate_mode{
        active = WA_ACTIVE,
        click_active = WA_CLICKACTIVE,
        inactive = WA_INACTIVE,
      };

      callback<void(window * sender)> OnActivate;
      callback<void(window * sender)> OnDeactivate;

      void handle_msg(wtf::window_message& msg) override {
        if (WM_ACTIVATE == msg.umsg) on_wm_activate(static_cast<activate_mode>(LOWORD(msg.wparam)), HIWORD(msg.wparam) ? true : false, reinterpret_cast<HWND>(msg.lparam));
      }

    protected:

      virtual void on_wm_activate(activate_mode mode, bool minimized, HWND target){
        if (activate_mode::inactive == mode) OnDeactivate(this);
        else OnActivate(this);
      }



      explicit wm_activate(window * pParent) noexcept : _SuperT(pParent){}

    };

  }
}