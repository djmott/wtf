﻿/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#define DOXY_INHERIT_WM_COMMAND

namespace wtf {
  namespace messages {

    /** @class wm_command
    @brief WM_COMMAND message handler
    @ingroup Messages
    */
    template <typename _super_t>
    struct wm_command : _super_t {

      //! @brief invoked as a result of a WM_COMMAND message.
      callback<void(window * sender, WPARAM, LPARAM)> OnCommand;

    protected:
      template <typename, template <typename> typename...> friend struct window_impl;

      //! @brief invokes the OnCommand callback as the result of a WM_COMMAND message.
      virtual void on_wm_command(WPARAM wparam,LPARAM lparam) { OnCommand(this, wparam, lparam); }


#if !DOXY_INVOKED
      void handle_msg(wtf::window_message& msg) override {
        if (WM_COMMAND != msg.umsg) return;
        on_wm_command(msg.wparam, msg.lparam);        
      }
#endif
    };

  }
}