/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {

    namespace _ {
      TCHAR sDATETIMEPICK_CLASS[] = DATETIMEPICK_CLASS;
    }

    /** @class date_time
    @ingroup Widgets
    @brief A standard calendar to select a date and time
    */
    struct date_time : window_impl<date_time,
      policy::has_font,
      policy::has_text,
      wtf::policy::has_move
    > {};

  }

  template <WNDPROC window_proc>
  struct window_class<controls::date_time, window_proc> :
    super_window_class<controls::_::sDATETIMEPICK_CLASS, controls::date_time, window_proc> {};

}