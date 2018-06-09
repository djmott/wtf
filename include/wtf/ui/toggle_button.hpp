/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{

    template <typename _super_t>
    struct isa_toggle_button : _super_t{

      bool value() const noexcept { return _value; }
      void value(bool newval){
        if (newval == _value) return;
        _value = newval;
        _super_t::border_style((_value ? border_styles::lowered : border_styles::raised));
      }

      explicit isa_toggle_button(window * pParent) noexcept : _super_t(pParent) {}

    protected:
      
      void on_wm_create() override{
        _super_t::border_style(border_styles::raised);
        _super_t::on_wm_create();
      };

      void on_wm_click(const mouse_msg_param<coord_frame::client>& m) override{
        if (mouse_buttons::left == m.button) value(!_value);
        _super_t::on_wm_click(m);
      };
      
    private:

      bool _value = false;
    };
  }




  struct toggle_button : window_impl<toggle_button, 
    policy::isa_toggle_button,
    policy::isa_label,
    policy::has_border,
    policy::has_invalidate,
    policy::has_text,
    policy::has_move,
    policy::has_invalidate,
    policy::has_click,
    policy::has_font,
    policy::has_invalidate,
    policy::has_background,
    policy::wm_erasebkgnd,
    policy::wm_mouse_down,
    policy::wm_mouse_up,
    policy::wm_paint,
    policy::wm_ncpaint,
    policy::wm_nccalcsize,
    policy::wm_create
  >{
    explicit toggle_button(window * pParent) noexcept : window_impl(pParent){}
  };

}
