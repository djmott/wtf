/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#define DOXY_INHERIT_DATE_TIME_SUPER \
  DOXY_INHERIT_WINDOW \
  DOXY_INHERIT_HAS_FONT \
  DOXY_INHERIT_HAS_TEXT \
  DOXY_INHERIT_HAS_MOVE

namespace wtf {
  namespace controls {


    /** @class date_picker
    @brief Select a date from a drop down calendar or enter a date in a text box
    @ingroup Controls
    @image html date_picker.png
    */
    struct date_picker : DOXY_INHERIT_DATE_TIME_SUPER window_impl<date_picker,
      policy::has_font,
      policy::has_text,
      policy::has_move
    > {
    
    };

    /** @class time_picker
    @brief Enter a time in a formatted text box with up-down arrows
    @ingroup Controls
    @image html time_picker.png
    */
    struct time_picker : date_picker {
      static constexpr DWORD Style = window::Style | DTS_TIMEFORMAT | DTS_UPDOWN;
    };


  }

#if !DOXY_INVOKED

  namespace _ {
    TCHAR sDATETIMEPICK_CLASS[] = DATETIMEPICK_CLASS;
  }

  template <WNDPROC window_proc>
  struct window_class<controls::time_picker, window_proc> :
    super_window_class<_::sDATETIMEPICK_CLASS, controls::time_picker, window_proc> {};

  template <WNDPROC window_proc>
  struct window_class<controls::date_picker, window_proc> :
    super_window_class<_::sDATETIMEPICK_CLASS, controls::date_picker, window_proc> {};

#endif

}