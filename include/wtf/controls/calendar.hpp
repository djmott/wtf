/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#define DOXY_INHERIT_CALENDAR_SUPER \
  DOXY_INHERIT_WINDOW \
  DOXY_INHERIT_HAS_TEXT \
  DOXY_INHERIT_HAS_FONT \
  DOXY_INHERIT_HAS_ENABLE \
  DOXY_INHERIT_HAS_MOVE \
  DOXY_INHERIT_HAS_SIZE \
  DOXY_INHERIT_WM_SIZE \
  DOXY_INHERIT_WM_COMMAND 

namespace wtf {
  namespace controls {


    /** @class calendar
    @brief A simple calendar control
    @ingroup Controls
    @image html calendar.png
    */
    struct calendar : DOXY_INHERIT_CALENDAR_SUPER window_impl<calendar,
      policy::has_text,
      policy::has_font,
      policy::has_enable,
      policy::has_move,
      policy::has_size,
      policy::wm_size,
      policy::wm_command
    > {
    
      rect<coord_frame::client> minimum_size() const {
        rect<coord_frame::client> r;
        wtf::exception::throw_lasterr_if(::SendMessage(*this, MCM_GETMINREQRECT, 0, reinterpret_cast<LPARAM>(&r)), [](LRESULT L) {return !L; });
        return r;
      }
    };

  }

#if !DOXY_INVOKED
  namespace _ {
    TCHAR sMONTHCAL_CLASS[] = MONTHCAL_CLASS;
  }

  template <WNDPROC window_proc> struct window_class<controls::calendar, window_proc>
    : super_window_class<_::sMONTHCAL_CLASS, controls::calendar, window_proc> {};
#endif

}
