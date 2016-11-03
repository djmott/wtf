/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  enum class icon_type{
    big_icon = ICON_BIG,
    small_icon = ICON_SMALL,
    small2_icon = ICON_SMALL2,
  };

  namespace policy{
    template <typename _SuperT>
    struct wm_geticon : _SuperT{
    protected:

      virtual HICON on_wm_geticon(icon_type) = 0;

      explicit wm_geticon(window * pParent) : _SuperT(pParent){}

      void handle_msg(_::window_message& msg) override{
        if (WM_GETICON == msg.umsg){
          msg.lresult = reinterpret_cast<LRESULT>(on_wm_geticon(static_cast<icon_type>(msg.wparam)));
          msg.bhandled = true;
        }
        _SuperT::handle_msg(msg);
      }
    };
  }
}