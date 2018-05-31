/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{
    template <typename _SuperT>
    struct wm_move : _SuperT{

      enum class wm_size_flags{
        hide = SIZE_MAXHIDE,
        maximized = SIZE_MAXIMIZED,
        show = SIZE_MAXSHOW,
        minimized = SIZE_MINIMIZED,
        restored = SIZE_RESTORED,
      };

      callback<void(window * sender, const point<coord_frame::client>&)> OnMove;

      void handle_msg(wtf::window_message& msg) override {
        if (WM_MOVE == msg.umsg) on_wm_move(point<coord_frame::client>(LOWORD(msg.lparam), HIWORD(msg.lparam)));
      }

    protected:

      virtual void on_wm_move(const point<coord_frame::client>& param){ OnMove(this, param); }

      explicit wm_move(window * pParent) : _SuperT(pParent){}

    };

  }
}