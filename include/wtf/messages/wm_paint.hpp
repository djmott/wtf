/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT>
    struct wm_paint : _SuperT{

    protected:

      virtual void on_wm_paint(const _::device_context&, const _::paint_struct&) {}

      explicit wm_paint(window * pParent) : _SuperT(pParent){}

      void handle_msg(_::window_message& msg) override{
        if (WM_PAINT == msg.umsg){
          msg.lresult = 0;
          msg.bhandled = true;
          auto & dc = *reinterpret_cast<const _::device_context *>(msg.wparam);
          auto & ps = *reinterpret_cast<const _::paint_struct *>(msg.lparam);
          on_wm_paint(dc, ps);
        }
        _SuperT::handle_msg(msg);
      }

    };
  }
}