/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#define DOXY_INHERIT_CALENDAR_SUPER \
DOXY_INHERIT_HAS_TEXT \
DOXY_INHERIT_HAS_FONT \
DOXY_INHERIT_HAS_ENABLE \
DOXY_INHERIT_HAS_MOVE \
DOXY_INHERIT_HAS_SIZE \
DOXY_INHERIT_WM_SIZE \
DOXY_INHERIT_WM_COMMAND \

namespace wtf {
  namespace controls {

    namespace _ {
      TCHAR sMONTHCAL_CLASS[] = MONTHCAL_CLASS;
    }

    /** @class calendar
    @brief A simple calendar control
    @ingroup Widgets
    */
    struct calendar : DOXY_INHERIT_CALENDAR_SUPER window_impl<calendar,
      policy::has_text,
      policy::has_font,
      policy::has_enable,
      policy::has_move,
      policy::has_size,
      policy::wm_size,
      policy::wm_command
    > {};

  }


  template <WNDPROC window_proc> struct window_class<controls::calendar, window_proc> : super_window_class<controls::_::sMONTHCAL_CLASS, controls::calendar, window_proc> {};

}
