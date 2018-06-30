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

      static constexpr TCHAR sub_window_class_name[] = DATETIMEPICK_CLASS; 
      static constexpr TCHAR window_class_name[] = _T("wtf_date_picker"); 
      template <WNDPROC wp> using window_class_type = super_window_class<window_class_name, sub_window_class_name, wp>;

    };

    /** @class time_picker
    @brief Enter a time in a formatted text box with up-down arrows
    @ingroup Controls
    @image html time_picker.png
    */
    struct time_picker : window_impl<time_picker,
      policy::has_font,
      policy::has_text,
      policy::has_move
    > {
      static constexpr DWORD Style = window::Style | DTS_TIMEFORMAT | DTS_UPDOWN;
      static constexpr TCHAR sub_window_class_name[] = DATETIMEPICK_CLASS; 
      static constexpr TCHAR window_class_name[] = _T("wtf_time_picker"); 
      template <WNDPROC wp> using window_class_type = super_window_class<window_class_name, sub_window_class_name, wp>;
    };


  }
  
}