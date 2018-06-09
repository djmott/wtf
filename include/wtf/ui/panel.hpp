/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{
    template <typename _super_t>
    struct isa_panel : _super_t{

    protected:

      explicit isa_panel(window * pParent) noexcept : _super_t(pParent){}

    };
  }


  struct panel : window_impl<panel, 
    policy::isa_panel,
    policy::has_enable, 
    policy::has_border,
    policy::has_click,
    policy::has_show,
    policy::has_text,
    policy::has_size, 
    policy::has_move, 
    policy::has_font,
    policy::has_background,
    policy::has_invalidate,
    policy::has_zorder,
    policy::wm_erasebkgnd,
    policy::wm_enable, 
    policy::wm_size,
    policy::wm_paint,
    policy::wm_create,
    policy::wm_mouse_down,
    policy::wm_mouse_up,
    policy::wm_ncpaint,
    policy::wm_nccalcsize
  >{
    explicit panel(window * pParent) noexcept : window_impl(pParent){}
  };

}
