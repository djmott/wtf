/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {

    namespace _ {

      TCHAR sDATETIMEPICK_CLASS[] = DATETIMEPICK_CLASS;

      template <typename _impl_t> using date_time_impl = window_impl<_impl_t,
        policy::has_font,
        policy::has_text,
        wtf::policy::has_move
      >;

    }

    struct date_time : _::date_time_impl<date_time> {

      
      date_time(window * parent) : _::date_time_impl<date_time>(parent) {
        wtf::_::init_common_controls<wtf::_::date_classes>::get();
      }


    };

  }

  template <WNDPROC window_proc>
  struct window_class<controls::date_time, window_proc> :
    super_window_class<controls::_::sDATETIMEPICK_CLASS, controls::date_time, window_proc> {};

}