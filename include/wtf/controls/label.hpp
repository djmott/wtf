/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {

    namespace policy {
      
      template <typename _impl_t> using label_super_t = window_impl<_impl_t,
        policy::has_text,
        policy::has_font,
        wtf::policy::has_enable,
        wtf::policy::has_move,
        wtf::policy::has_size,
        wtf::policy::wm_size,
        wtf::policy::wm_command
      >;
    }
    
    /** @class label
    @ingroup Widgets
    @brief A static control with text.
    */
    struct label : window_impl<label,
      policy::has_text,
      policy::has_font,
      wtf::policy::has_enable,
      wtf::policy::has_move,
      wtf::policy::has_size,
      wtf::policy::wm_size,
      wtf::policy::wm_command
    > {};
    
  }

  namespace _ { TCHAR sWC_STATIC[] = WC_STATIC; }

  template <WNDPROC window_proc> struct window_class<controls::label, window_proc> : super_window_class<_::sWC_STATIC, controls::label, window_proc> {};

}
