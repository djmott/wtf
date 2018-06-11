/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace custom {
    namespace policy {

      template <typename _super_t>
      struct isa_toggle_button : _super_t {

        bool value() const noexcept { return _value; }
        void value(bool newval) {
          if (newval == _value) return;
          _value = newval;
          _super_t::border_style((_value ? border_styles::lowered : border_styles::raised));
        }

        explicit isa_toggle_button(window * pParent) : _super_t(pParent) {}

      protected:

        void on_wm_create() override {
          _super_t::border_style(border_styles::raised);
          _super_t::on_wm_create();
        };

        void on_wm_click(const mouse_msg_param<coord_frame::client>& m) override {
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
      wtf::policy::has_border,
      wtf::policy::has_invalidate,
      wtf::policy::has_text,
      wtf::policy::has_move,
      wtf::policy::has_invalidate,
      wtf::policy::has_click,
      wtf::policy::has_font,
      wtf::policy::has_invalidate,
      wtf::policy::has_background,
      wtf::policy::wm_erasebkgnd,
      wtf::policy::wm_mouse_down,
      wtf::policy::wm_mouse_up,
      wtf::policy::wm_paint,
      wtf::policy::wm_ncpaint,
      wtf::policy::wm_nccalcsize,
      wtf::policy::wm_create
    > {
      explicit toggle_button(window * pParent) : window_impl(pParent) {}
    };

  }
}