/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#define DOXY_INHERIT_DATE_TIME_SUPER \
  DOXY_INHERIT_HAS_FONT \
  DOXY_INHERIT_HAS_TEXT \
  DOXY_INHERIT_HAS_MOVE

namespace wtf {
  namespace controls {


    /** @class date_time
    @brief A standard calendar to select a date and time
    @ingroup Widgets
    @image html date_time.png
    */
    struct date_time : DOXY_INHERIT_DATE_TIME_SUPER window_impl<date_time,
      policy::has_font,
      policy::has_text,
      policy::has_move
    > {};

  }

#if !DOXY_INVOKED
  namespace _ {
    TCHAR sDATETIMEPICK_CLASS[] = DATETIMEPICK_CLASS;
  }

  template <WNDPROC window_proc>
  struct window_class<controls::date_time, window_proc> :
    super_window_class<_::sDATETIMEPICK_CLASS, controls::date_time, window_proc> {};
#endif
}