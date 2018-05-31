/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{

    template <typename _SuperT>
    struct wm_paint : _SuperT{

      void handle_msg(wtf::window_message& msg) override {
        if (WM_PAINT == msg.umsg) {
          msg.lresult = 0;
          msg.bhandled = true;
          auto & dc = *reinterpret_cast<const wtf::_::device_context *>(msg.wparam);
          auto & ps = *reinterpret_cast<const wtf::_::paint_struct *>(msg.lparam);
          on_wm_paint(dc, ps);
        }

      }

    protected:

      virtual void on_wm_paint(const wtf::_::device_context& dc, const wtf::_::paint_struct& ps)  { }

      explicit wm_paint(window * pParent) noexcept : _SuperT(pParent){}

    };
  }
}