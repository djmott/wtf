/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{
    template <typename _super_t>
    struct wm_erasebkgnd : _super_t{
      
      callback<void(window * sender, const device_context&, const rect<coord_frame::client>&)> OnEraseBackground;

    protected:
      template <typename, template <typename> typename...> friend struct window_impl;

      virtual void on_wm_erasebkgnd(const device_context& dc, const rect<coord_frame::client>& rc) {
        OnEraseBackground(this, dc, rc);
      }

      explicit wm_erasebkgnd(window * pParent) noexcept : _super_t(pParent){}

      void handle_msg(wtf::window_message& msg) override {
        if (WM_ERASEBKGND != msg.umsg) return;
        on_wm_erasebkgnd(device_context::get_client(*this), rect<coord_frame::client>::get(*this));
        msg.lresult = TRUE;
        msg.bhandled = true;
      }

    };
  }
}