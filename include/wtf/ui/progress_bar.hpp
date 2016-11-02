/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once


namespace wtf{
  namespace policy{
    template <typename _SuperT>
    struct isa_progressbar :  _SuperT{


      int min() const{ return _min; }
      void min(int newval){
        _min = newval;
        _SuperT::invalidate();
      }

      int max() const{ return _max; }
      void max(int newval){
        _max = newval;
        _SuperT::invalidate();
      }

      int value() const{ return _value; }
      void value(int newval){
        _value = newval;
        _SuperT::invalidate();
      }

      rgb fill_color() const{ return _fill_color; }
      void fill_color(rgb newval){
        _fill_color = newval;
        _SuperT::invalidate();
      }

      enum class text_modes{
        none,
        value,
        percentage,
      };

      text_modes text_mode() const{ return _text_mode; }
      void text_mode(text_modes newval){
        _text_mode = newval;
        _SuperT::invalidate();
      }
    protected:

      explicit isa_progressbar(iwindow * hParent) : _SuperT(hParent){}

      void on_wm_create() override{
        _SuperT::border_style(border_styles::lowered);
        _SuperT::auto_draw_text(false);
        _SuperT::on_wm_create();
      };


      int _min = 0;
      int _max = 100;
      int _value = 0;
      rgb _fill_color = system_rgb<system_colors::highlight>();
      text_modes _text_mode = text_modes::percentage;

      void on_wm_paint(const _::device_context& dc, const _::paint_struct& ps) override{
        auto oBrush = brush::solid_brush(_fill_color);
        rect<coord_frame::client> oFillArea = ps.client();
        auto iExtent = _max - _min;
        if (orientations::horizontal == _SuperT::orientation()) {
          oFillArea.right = (oFillArea.right * _value) / iExtent;
        } else{
          oFillArea.top = (oFillArea.top * _value) / iExtent;
        }
        dc.fill(oFillArea, oBrush);
        if (text_modes::none == _text_mode) return _SuperT::on_wm_paint(dc, ps);
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
        _SuperT::text(sDisplayText);
        oFillArea.right = std::max(oTextSize.cx, oFillArea.right);
        _SuperT::text_vertical_alignment(text_vertical_alignments::center);
        _SuperT::text_horizontal_alignment(text_horizontal_alignments::center);
        _SuperT::on_wm_paint(dc, ps);
      }


    };
  }


  template <> struct policy_traits<policy::isa_progressbar>{
    using requires = policy_list<policy::has_orientation, policy::isa_label>;
  };

  struct progress_bar : window_impl<progress_bar, policy::isa_progressbar>{
    explicit progress_bar(iwindow * pParent) : window_impl(pParent){}
  };


}
