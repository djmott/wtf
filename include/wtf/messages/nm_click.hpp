/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once
#define DOXY_INHERIT_NM_CLICK

namespace wtf {
  namespace messages {
    template <typename _super_t>
    struct nm_click: _super_t {

      callback<void(window * sender)> OnClick;

    protected:

      template <typename, template <typename> typename...> friend struct window_impl;

      virtual void on_nm_click() { OnClick(this); }

      void handle_msg(wtf::window_message& msg) override {
        if (WM_NOTIFY != msg.umsg) return;
        const auto nmhdr = reinterpret_cast<NMHDR*>(msg.lparam);
        if (NM_CLICK == nmhdr->code && nmhdr->hwndFrom == _super_t::_handle) on_nm_click();
      }

    };
  }
}