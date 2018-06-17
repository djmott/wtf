/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {

    namespace _ { TCHAR sWC_STATIC[] = WC_STATIC; }

    /** @class label
    @ingroup Widgets
    @brief A static control with text.
    */
    struct label : window_impl<label,
      policy::has_text,
      policy::has_font,
      policy::has_enable,
      policy::has_move,
      policy::has_size,
      policy::wm_size,
      policy::wm_command
    > {};
    
  }


  template <WNDPROC window_proc> struct window_class<controls::label, window_proc> : super_window_class<controls::_::sWC_STATIC, controls::label, window_proc> {};

}
