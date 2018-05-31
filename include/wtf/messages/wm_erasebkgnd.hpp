/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT>
    struct wm_erasebkgnd : _SuperT{
      
      callback<void(window * sender, const wtf::_::device_context&, const rect<coord_frame::client>&)> OnEraseBackground;

      void handle_msg(wtf::window_message& msg) override {
        if (WM_ERASEBKGND != msg.umsg) return;
        on_wm_erasebkgnd(*reinterpret_cast<const wtf::_::device_context *>(msg.lparam), rect<coord_frame::client>::get(*this));
        msg.lresult = TRUE;
        msg.bhandled = true;
      }

    protected:

      virtual void on_wm_erasebkgnd(const wtf::_::device_context& dc, const rect<coord_frame::client>& rc) {
        OnEraseBackground(this, dc, rc);
      }

      explicit wm_erasebkgnd(window * pParent) : _SuperT(pParent){}

    };
  }
}