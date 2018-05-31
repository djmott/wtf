/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{
    template <typename _SuperT>
    struct wm_sizing : _SuperT{

      callback<void(window * sender, rect<coord_frame::screen>&)> OnSizing;

      void handle_msg(wtf::window_message& msg) override {
        if (WM_SIZING == msg.umsg) on_wm_sizing(*reinterpret_cast<rect<coord_frame::screen>*>(msg.lparam));

      }
    protected:
      virtual void on_wm_sizing(rect<coord_frame::screen>& param){ OnSizing(this, param); }

      explicit wm_sizing(window * pParent) : _SuperT(pParent){}


    };

  }
}
