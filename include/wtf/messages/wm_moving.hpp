/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT>
    struct wm_moving : _SuperT{

      callback<void(window * sender, rect<coord_frame::screen>&)> OnMoving;

    protected:
      template <typename, template <typename> typename...> friend struct window_impl;

      virtual void on_wm_moving(rect<coord_frame::screen>& param){ OnMoving(this, param); }

      explicit window(window * pParent) : _SuperT(pParent){}

      void handle_msg(wtf::window_message& msg) override {
        if (WM_MOVING == msg.umsg) on_wm_moving(*reinterpret_cast<rect<coord_frame::screen>*>(msg.lparam));
      }

    };

  }
}