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

      callback<void()> OnActivate;
      callback<void()> OnDeactivate;

    protected:

      virtual void on_wm_activate(activate_mode mode, bool minimized, HWND target){
        if (activate_mode::inactive == mode) OnDeactivate();
        else OnActivate();
      }

      void handle_msg(_::window_message& msg) override{
        if (WM_ACTIVATE == msg.umsg) on_wm_activate(static_cast<activate_mode>(LOWORD(msg.wparam)), HIWORD(msg.wparam) ? true : false, reinterpret_cast<HWND>(msg.lparam));
        _SuperT::handle_msg(msg);
      }


      explicit wm_activate(window * pParent) : _SuperT(pParent){}

    };

  }
}