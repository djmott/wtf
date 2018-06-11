/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{

    template <typename _checkbox_super_t>
    struct isa_checkbox : _checkbox_super_t {
    protected:

      explicit isa_checkbox(window * pParent) : _checkbox_super_t(pParent), _check(this){
        _checkbox_super_t::auto_draw_text(false);
        _checkbox_super_t::border_style(border_styles::none);
      }

      void on_wm_click(const mouse_msg_param<coord_frame::client>& m) override{
        if (mouse_buttons::left == m.button){
          _check.value(!_check.value());
        }
        _checkbox_super_t::on_wm_click(m);
      };

      void on_wm_paint(const device_context& dc, const paint_struct& ps) override{
        auto client = ps.client();
        auto TextSize = _checkbox_super_t::prefered_text_size();
        if (text_horizontal_alignments::left == _checkbox_super_t::text_horizontal_alignment()){
          _check.move(0, (client.bottom - checkbox_size) / 2, checkbox_size, checkbox_size);
          _checkbox_super_t::draw_text(dc, rect<coord_frame::client>(checkbox_size, 0, client.right - checkbox_size, client.bottom));
        } else{
          _check.move(client.right - checkbox_size, (client.bottom - checkbox_size) / 2, checkbox_size, checkbox_size);
          _checkbox_super_t::draw_text(dc, rect<coord_frame::client>(0, 0, client.right - checkbox_size, client.bottom));
        }
        _checkbox_super_t::on_wm_paint(dc, ps);
      };
      
      static const int checkbox_size = 15;

      template <typename _check_t> using _check_super_t = window_impl<_check_t,
        policy::isa_panel,
        policy::has_click,
        policy::has_move,
        policy::has_border,
        policy::has_invalidate,
        policy::wm_paint,
        policy::wm_mouse_down,
        policy::wm_mouse_up,
        policy::wm_paint,
        policy::wm_ncpaint,
        policy::wm_nccalcsize
      >;

      struct _check : _check_super_t<_check>
      {

        _check(isa_checkbox * pParent) : _check_super_t<_check>(pParent),_parent(pParent){
          _check_super_t<_check>::border_style(border_styles::raised);
        }

        void on_wm_click(const mouse_msg_param<coord_frame::client>& m) override{
          if (mouse_buttons::left == m.button) value(!_value);
          _check_super_t<_check>::on_wm_click(m);
        }

        void on_wm_paint(const device_context& dc, const paint_struct& ps) override{
          auto client = ps.client();
          dc.fill(client, brush::solid_brush(rgb(255, 255, 255)));
          if (_value){
            client.top += 2;
            client.left += 2;
            client.bottom -= 3;
            client.right -= 3;
            auto black = pen::create(pen::style::solid, 2, rgb(0, 0, 0));
            dc.line(black, client.left, client.top, client.right, client.bottom);
            dc.line(black, client.right, client.top, client.left, client.bottom);
          }
          _check_super_t<_check>::on_wm_paint(dc, ps);
        }
        

        bool value() const noexcept { return _value; }
        void value(bool newval){
          _value = newval;
          _check_super_t<_check>::border_style(newval ? border_styles::lowered : border_styles::raised);
          _check_super_t<_check>::invalidate();
        }

      private:
        bool _value = false;
        isa_checkbox * _parent;
      }_check;

    };
  }



  struct checkbox : window_impl<checkbox,
    policy::isa_checkbox, 
    policy::isa_label,
    policy::has_click,
    policy::has_text,
    policy::has_move,
    policy::has_border,
    policy::has_font,
    policy::has_invalidate,
    policy::wm_ncpaint,
    policy::wm_nccalcsize,
    policy::wm_erasebkgnd,
    policy::wm_mouse_down,
    policy::wm_mouse_up,
    policy::wm_paint
  >{
    explicit checkbox(window * pParent) : window_impl(pParent){}
  };

}
