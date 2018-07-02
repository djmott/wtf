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


    /** @class date_time_picker
    @brief Select a date or time. The date style includes a drop-down calendar.
    @ingroup Controls
    @image html date_picker.png
    @image html time_picker.png
    */
    struct date_time_picker : DOXY_INHERIT_DATE_TIME_SUPER window_impl<date_time_picker,
      policy::has_font,
      policy::has_text,
      policy::has_move
    > {

      enum class styles {
        date,
        time,
      };
      //! @brief gets the style
      styles style() const {
        return (window::_style & DTS_TIMEFORMAT ? styles::time : styles::date);
      }
      //! @brief sets the style
      //! @brief this setting can only be changed prior to calling run()
      void style(styles newval) {
        window::_style &= ~(DTS_TIMEFORMAT | DTS_UPDOWN);
        window::_style |= (styles::time == newval ? (DTS_TIMEFORMAT | DTS_UPDOWN) : 0);
      }

    protected:
      template <typename, template <typename> typename...> friend struct window_impl;
      void add(window &) override {}
      static constexpr TCHAR sub_window_class_name[] = DATETIMEPICK_CLASS;
      static constexpr TCHAR window_class_name[] = _T("wtf_date_time_picker"); 
      template <WNDPROC wp> using window_class_type = super_window_class<window_class_name, sub_window_class_name, wp>;

    };

  }
  
}