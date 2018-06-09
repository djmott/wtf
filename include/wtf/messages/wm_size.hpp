/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{
    /** 
    @class wm_size 
    captures WM_SIZE message and produces OnSize event
    @ingroup Messages
    */
    template <typename _super_t>
    struct wm_size : _super_t{

      /**
      @var OnSize The window has been resized
      @ingroup Callbacks
      */
      callback<void(window * sender, const point<coord_frame::client>&)> OnSize;

    protected:
      template <typename, template <typename> typename...> friend struct window_impl;

      explicit wm_size(window * pParent) noexcept : _super_t(pParent){}

      virtual void on_wm_size(const point<coord_frame::client>& p){ OnSize(this, p); }

      void handle_msg(wtf::window_message& msg) override {
        if (WM_SIZE == msg.umsg) on_wm_size(point<coord_frame::client>(LOWORD(msg.lparam), HIWORD(msg.lparam)));

      }

    };

  }
}