/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT>
    struct wm_size : _SuperT{

      callback<void(const point<coord_frame::client>&)> OnSize;

    protected:

      explicit wm_size(iwindow * pParent) : _SuperT(pParent){}

      virtual void on_wm_size(const point<coord_frame::client>& p){ OnSize(p); }

      void handle_msg(_::window_message& msg) override{
        if (WM_SIZE == msg.umsg) on_wm_size(point<coord_frame::client>(LOWORD(msg.lparam), HIWORD(msg.lparam)));
        _SuperT::handle_msg(msg);
      }

    };

  }
}