/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT>
    struct wm_nccalcsize : _SuperT{

      enum class activate_mode{
        active = WA_ACTIVE,
        click_active = WA_CLICKACTIVE,
        inactive = WA_INACTIVE,
      };

    protected:

      virtual LRESULT on_wm_nccalcsize(NCCALCSIZE_PARAMS *) = 0;
      virtual LRESULT on_wm_nccalcsize(RECT *){ return 0; }

      explicit wm_nccalcsize(window * pParent) : _SuperT(pParent){}

      void handle_msg(_::window_message& msg) override{
        if (WM_NCCALCSIZE == msg.umsg){
          msg.bhandled = true;
          if (msg.wparam) msg.lresult = on_wm_nccalcsize(reinterpret_cast<NCCALCSIZE_PARAMS*>(msg.lparam));
          else  msg.lresult = on_wm_nccalcsize(reinterpret_cast<RECT*>(msg.lparam));

        }
        _SuperT::handle_msg(msg);
      }

    };
  }
}