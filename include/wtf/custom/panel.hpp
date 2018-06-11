/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace custom {

    namespace policy {
      template <typename _super_t>
      struct isa_panel : _super_t {

      protected:

        explicit isa_panel(window * pParent) : _super_t(pParent) {}

      };
    }


    struct panel : window_impl < panel,
      policy::isa_panel,
      wtf::policy::has_enable,
      wtf::policy::has_border,
      wtf::policy::has_click,
      wtf::policy::has_show,
      wtf::policy::has_text,
      wtf::policy::has_size,
      wtf::policy::has_move,
      wtf::policy::has_font,
      wtf::policy::has_background,
      wtf::policy::has_invalidate,
      wtf::policy::has_zorder,
      wtf::policy::wm_erasebkgnd,
      wtf::policy::wm_enable,
      wtf::policy::wm_size,
      wtf::policy::wm_paint,
      wtf::policy::wm_create,
      wtf::policy::wm_mouse_down,
      wtf::policy::wm_mouse_up,
      wtf::policy::wm_ncpaint,
      wtf::policy::wm_nccalcsize
    > {
      explicit panel(window * pParent) : window_impl(pParent) {}
    };

  }
}