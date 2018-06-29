/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#define DOXY_INHERIT_WM_SIZE

namespace wtf{

  namespace policy{
    /** @class wm_size 
    @brief WM_SIZE message handler
    @ingroup Messages
    */
    template <typename _super_t>
    struct wm_size : _super_t{

      /** @enum wm_size_flags
      @brief indicates the type of the size change
      */
      enum class wm_size_flags {
        hide = SIZE_MAXHIDE, //!< the window is being hidden as the result of another window being maximized
        maximized = SIZE_MAXIMIZED, //!< the window has been maximized
        show = SIZE_MAXSHOW, //!< another maximized window has been restored
        minimized = SIZE_MINIMIZED, //!< the window has been minimized
        restored = SIZE_RESTORED, //!< the window has been restored
      };

      //! @brief invoked as the result of a size change
      callback<void(window * sender, wm_size_flags, const point<coord_frame::client>&)> OnSize;

    protected:
      template <typename, template <typename> typename...> friend struct window_impl;

      //! @brief invokes the OnSize callback as a result of a WM_SIZE message
      virtual void on_wm_size(wm_size_flags flags, const point<coord_frame::client>& p){
        OnSize(this, flags, p);
      }
#if !DOXY_INVOKED
      void handle_msg(wtf::window_message& msg) override {
        if (WM_SIZE == msg.umsg) on_wm_size(static_cast<wm_size_flags>(msg.wparam), point<coord_frame::client>(LOWORD(msg.lparam), HIWORD(msg.lparam)));
      }
#endif
    };

  }
}