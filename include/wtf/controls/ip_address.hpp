/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {

    namespace _ {

      TCHAR sWC_IPADDRESS[] = WC_IPADDRESS;

      template <typename _impl_t> using ip_address_impl = window_impl<_impl_t,
        policy::has_font,
        policy::has_text,
        wtf::policy::has_move
      >;

    }

    struct ip_address : _::ip_address_impl<ip_address> {

      ip_address(window * parent) : _::ip_address_impl<ip_address>(parent) {
        wtf::_::init_common_controls<wtf::_::internet_classes>::get();
      }


    protected:
      void handle_msg(wtf::window_message& msg) override {
        if (WM_COMMAND != msg.umsg) return;
        //this control must not forward parent redirected WM_COMMAND messages
        msg.bhandled = (msg.hwnd != _handle);
        if (!msg.bhandled) DebugBreak();
      }

    };

  }

  template <WNDPROC window_proc>
  struct window_class<controls::ip_address, window_proc> :
    super_window_class<controls::_::sWC_IPADDRESS, controls::ip_address, window_proc> {};

}