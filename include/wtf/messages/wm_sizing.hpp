/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{
    template <typename _super_t>
    struct wm_sizing : _super_t{

      callback<void(window * sender, rect<coord_frame::screen>&)> OnSizing;

    protected:
      template <typename, template <typename> typename...> friend struct window_impl;
      virtual void on_wm_sizing(rect<coord_frame::screen>& param) { OnSizing(this, param); }

      explicit wm_sizing(window * pParent) : _super_t(pParent){}

      void handle_msg(wtf::window_message& msg) override {
        if (WM_SIZING == msg.umsg) on_wm_sizing(*reinterpret_cast<rect<coord_frame::screen>*>(msg.lparam));

      }

    };

  }
}
