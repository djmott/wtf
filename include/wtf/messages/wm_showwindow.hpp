/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{
    template <typename _super_t>
    struct wm_showwindow : _super_t{

      enum class visibility_change_flag{
        show_window = 0,
        other_unzoom = SW_OTHERUNZOOM,
        other_zoom = SW_OTHERZOOM,
        parent_closing = SW_PARENTCLOSING,
        parent_opening = SW_PARENTOPENING,
      };

      callback<void(window * sender, visibility_change_flag)> OnShow;

    protected:
      template <typename, template <typename> typename...> friend struct window_impl;

      explicit wm_showwindow(window * pParent) : _super_t(pParent){}

      virtual void on_wm_showwindow(visibility_change_flag f){ OnShow(this, f); }

      void handle_msg(wtf::window_message& msg) override {
        if (WM_SHOWWINDOW == msg.umsg) on_wm_showwindow(static_cast<visibility_change_flag>(msg.lparam));

      }

    };

  }
}