/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {

    namespace policy {

      template <typename _impl_t> using listview_super_t = window_impl<_impl_t,
        policy::has_text,
        policy::has_font,
        wtf::policy::has_enable,
        wtf::policy::has_move,
        wtf::policy::wm_command
      >;
    }

    struct listview : policy::listview_super_t<listview> {
      explicit listview() : policy::listview_super_t<listview>() {
        wtf::_::init_common_controls<wtf::_::listview_classes>::get();
      }
    };

  }

  namespace _ { TCHAR sWC_LISTVIEW[] = WC_LISTVIEW; }

  template <WNDPROC window_proc> struct window_class<controls::listview, window_proc> : super_window_class<_::sWC_LISTVIEW, controls::listview, window_proc> {};

}
