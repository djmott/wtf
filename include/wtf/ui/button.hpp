/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{

    template <typename _super_t>
    struct isa_button : _super_t{

    protected:

      explicit isa_button(window * hParent) : _super_t(hParent){
        _super_t::border_style(border_styles::raised);
      }
      
      void on_wm_mouse_down(const mouse_msg_param<coord_frame::client>& oParam) override{
        if (oParam.button == mouse_buttons::left){
          _super_t::border_style(border_styles::lowered);
          _super_t::invalidate();
          ::SetCapture(*this);
        }
        _super_t::on_wm_mouse_down(oParam);
      }

      void on_wm_mouse_up(const mouse_msg_param<coord_frame::client>& oParam) override{
        if (oParam.button == mouse_buttons::left){
          _super_t::border_style(border_styles::raised);
          _super_t::invalidate();
          ::ReleaseCapture();
        }
        _super_t::on_wm_mouse_up(oParam);
      }

    };
  }


  struct button : window_impl<button, 
    policy::isa_button, 
    policy::isa_label,
    policy::has_click,
    policy::has_border,
    policy::has_text,
    policy::has_move,
    policy::has_font,
    policy::has_invalidate,
    policy::has_background,
    policy::wm_erasebkgnd,
    policy::wm_mouse_down,
    policy::wm_mouse_up,
    policy::wm_ncpaint,
    policy::wm_nccalcsize,
    policy::wm_paint
  >{
    explicit button(window * pParent) : window_impl(pParent){}
  };

}
