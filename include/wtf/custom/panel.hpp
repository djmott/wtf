/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace custom {
    
    struct panel : window_impl < panel,
      wtf::policy::has_enable,
      wtf::policy::has_owner_drawn_border,
      wtf::policy::has_click,
      wtf::policy::has_show,
      wtf::policy::has_owner_drawn_text,
      wtf::policy::has_size,
      wtf::policy::has_move,
      wtf::policy::has_owner_drawn_font,
      wtf::policy::has_background,
      wtf::policy::has_invalidate,
      wtf::policy::has_zorder,
      wtf::messages::wm_erasebkgnd,
      wtf::messages::wm_enable,
      wtf::messages::wm_size,
      wtf::messages::wm_paint,
      wtf::messages::wm_create,
      wtf::messages::wm_mouse_down,
      wtf::messages::wm_mouse_up,
      wtf::messages::wm_ncpaint,
      wtf::messages::wm_nccalcsize
    > {
      static constexpr TCHAR window_class_name[] = _T("wtf_panel");
      template <WNDPROC wp> using window_class_type = window_class<window_class_name, wp>;
    };

  }
}