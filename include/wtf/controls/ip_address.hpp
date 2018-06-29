/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#if 0

#define DOXY_INHERIT_IPADDR_SUPER \
  DOXY_INHERIT_WINDOW \
  DOXY_INHERIT_HAS_FONT \
  DOXY_INHERIT_HAS_TEXT \
  DOXY_INHERIT_HAS_MOVE


namespace wtf {
  namespace controls {
    
    /** @class ip_address
    @brief Text box thats formatted to accept a IPv4 address
    @ingroup Widgets
    @image html ip_address.png
    */
    struct ip_address : DOXY_INHERIT_IPADDR_SUPER window_impl<ip_address,
      policy::has_font,
      policy::has_text,
      wtf::policy::has_move
    > {

      //! @brief clears the contents
      void clear() { ::SendMessage(*this, IPM_CLEARADDRESS, 0, 0); }

      //! @brief gets the address
      uint32_t address() const {
        uint32_t iRet = 0;
        ::SendMessage(*this, IPM_GETADDRESS, 0,reinterpret_cast<LPARAM>(&iRet));
        return iRet;
      }
      //! @brief sets the address
      void address(uint32_t newval) { ::SendMessage(*this, IPM_SETADDRESS, 0, newval); }

      //! @brief determines if all fields are blank
      bool is_blank() const { return (::SendMessage(*this, IPM_ISBLANK, 0, 0) ? true : false); }

    protected:
/*
      void handle_msg(wtf::window_message& msg) override {
        if (WM_COMMAND != msg.umsg) return;
        //this control must not forward parent redirected WM_COMMAND messages
        msg.bhandled = (msg.hwnd != _handle);
        if (!msg.bhandled) DebugBreak();
      }
*/

    };

  }

#if !DOXY_INVOKED

  namespace _ {
    TCHAR sWC_IPADDRESS[] = WC_IPADDRESS;
  }

  template <WNDPROC window_proc>
  struct window_class<controls::ip_address, window_proc> :
    super_window_class<_::sWC_IPADDRESS, controls::ip_address, window_proc> {};

#endif
}
#endif