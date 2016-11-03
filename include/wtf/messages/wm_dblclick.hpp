/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{
    template <typename _SuperT>
    struct wm_dblclick : _SuperT{

      callback<void(const mouse_msg_param&)> OnDblClick;
      
    protected:
      virtual void on_wm_dblclick(const mouse_msg_param& p){ OnDblClick(p); }

      explicit wm_dblclick(window * pParent) : _SuperT(pParent){}

      void handle_msg(_::window_message& msg) override{
        if (WM_LBUTTONDBLCLK == msg.umsg){
          on_wm_dblclick(mouse_msg_param(msg.wparam, msg.lparam, mouse_msg_param::buttons::left));
        } else if (WM_MBUTTONDBLCLK == msg.umsg){
          on_wm_dblclick(mouse_msg_param(msg.wparam, msg.lparam, mouse_msg_param::buttons::middle));
        } else if (WM_RBUTTONDBLCLK == msg.umsg){
          on_wm_dblclick(mouse_msg_param(msg.wparam, msg.lparam, mouse_msg_param::buttons::right));
        }
        _SuperT::handle_msg(msg);
      }
    };
  }
}
