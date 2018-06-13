/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{
    template <typename _super_t>
    struct wm_getminmaxinfo : _super_t{

      callback<void(window * sender, MINMAXINFO * param)> OnGetMinMaxInfo;

    protected:
      template <typename, template <typename> typename...> friend struct window_impl;

      explicit wm_getminmaxinfo(window * pParent) : _super_t(pParent) {}

      virtual void on_wm_getminmaxinfo(MINMAXINFO * param){
        OnGetMinMaxInfo(this, param);
      }


      void handle_msg(wtf::window_message& msg) override {
        if (WM_GETMINMAXINFO != msg.umsg) return;
        on_wm_getminmaxinfo(reinterpret_cast<MINMAXINFO*>(msg.lparam));
        msg.lresult = 0;
        msg.bhandled = true;
      }

    };
  }
}