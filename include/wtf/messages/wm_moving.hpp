/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT>
    struct wm_moving : _SuperT{

    protected:
      virtual void on_wm_moving(rect<coord_frame::screen>&){}

      explicit window(window * pParent) : _SuperT(pParent){}

      void handle_msg(_::window_message& msg) override{
        if (WM_MOVING == msg.umsg) on_wm_moving(*reinterpret_cast<rect<coord_frame::screen>*>(msg.lparam));
        _SuperT::handle_msg(msg);
      }

    };

  }
}