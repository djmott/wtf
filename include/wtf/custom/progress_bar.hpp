/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once
#if 0

namespace wtf{
  namespace custom{
  namespace policy{
    template <typename _super_t>
    struct isa_progressbar :  _super_t{


      int min() const{ return _min; }
      void min(int newval){
        _min = newval;
        _super_t::invalidate();
      }

      int max() const{ return _max; }
      void max(int newval){
        _max = newval;
        _super_t::invalidate();
      }

      int value() const{ return _value; }
      void value(int newval){
        _value = newval;
        _super_t::invalidate();
      }

      rgb fill_color() const{ return _fill_color; }
      void fill_color(rgb newval){
        _fill_color = newval;
        _super_t::invalidate();
      }

      enum class text_modes{
        none,
        value,
        percentage,
      };

      text_modes text_mode() const{ return _text_mode; }
      void text_mode(text_modes newval){
        _text_mode = newval;
        _super_t::invalidate();
      }
    protected:

      explicit isa_progressbar(window * hParent)  : _super_t(hParent){}

      void on_wm_create() override{
        _super_t::border_style(border_styles::lowered);
        _super_t::auto_draw_text(false);
        _super_t::on_wm_create();
      };


      int _min = 0;
      int _max = 100;
      int _value = 0;
      rgb _fill_color = system_rgb<system_colors::highlight>();
      text_modes _text_mode = text_modes::percentage;

      void on_wm_paint(const device_context& dc, const paint_struct& ps) override{
        auto oBrush = brush::solid_brush(_fill_color);
        rect<coord_frame::client> oFillArea = ps.client();
        auto iExtent = _max - _min;
        if (orientations::horizontal == _super_t::orientation()) {
          oFillArea.right = (oFillArea.right * _value) / iExtent;
        } else{
          oFillArea.top = (oFillArea.bottom * _value)/ iExtent;
        }
        dc.fill(oFillArea, oBrush);
        if (text_modes::none == _text_mode) return _super_t::on_wm_paint(dc, ps);
        //poor mans utf8 conversion...get with the program mingw
        tstring sDisplayText;
        std::string sTemp;
        if (text_modes::percentage == _text_mode){
          int iTemp = 100 - ((_value * 100) / (_max - _min));
          sTemp = std::to_string(iTemp);
          sTemp += "%";
        } else{
          sTemp = std::to_string(_value);
        }
        std::copy(sTemp.begin(), sTemp.end(), std::back_inserter(sDisplayText));
        auto oTextSize = dc.get_text_extent(sDisplayText);
        _super_t::text(sDisplayText);
        oFillArea.right = std::max(oTextSize.cx, oFillArea.right);
        _super_t::text_vertical_alignment(text_vertical_alignments::center);
        _super_t::text_horizontal_alignment(text_horizontal_alignments::center);
        _super_t::on_wm_paint(dc, ps);
      }

    };
  }

  struct progress_bar : window_impl<progress_bar,
    policy::isa_progressbar,
    policy::isa_label,
    policy::has_orientation,
    policy::has_move,
    policy::has_text,
    policy::has_font,
    policy::has_invalidate,
    policy::has_border,
    policy::wm_create,
    policy::wm_paint,
    policy::wm_erasebkgnd,
    policy::wm_nccalcsize,
    policy::wm_ncpaint
  >{
    explicit progress_bar(window * pParent)  : window_impl(pParent){}
  };


}
}
#endif