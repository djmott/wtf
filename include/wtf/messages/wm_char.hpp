/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{
    template <typename _SuperT>
    struct wm_char : _SuperT{

      callback<void(window * sender, UINT char_code, keyboard_msg_param)> OnChar;

    protected:

      virtual void on_wm_char(UINT char_code, keyboard_msg_param param){ OnChar(this, char_code, param); }

      wm_char(window * pParent) : _SuperT(pParent){}

      void handle_msg(_::window_message& msg) override{
        if (WM_CHAR == msg.umsg) on_wm_char(static_cast<UINT>(msg.wparam), *reinterpret_cast<keyboard_msg_param*>(&msg.lparam));
        _SuperT::handle_msg(msg);
      }

    };
  }
}