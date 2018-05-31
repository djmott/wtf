/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{

    /** has_border
    * Creates borders
    */
    template <typename _SuperT>
    struct has_border :  _SuperT{

      virtual int border_width() const{
        switch (_border_style){
          case border_styles::none: return 0;
          case border_styles::flat:
          case border_styles::raised:
          case border_styles::lowered: return 1;
          default: return 2;
        }
      }
      virtual const rgb& border_highlight() const{ return _border_highlight; }
      virtual void border_highlight(const rgb& newval){
        _border_highlight = newval;
        refresh_border();
      }

      virtual const rgb& border_shadow() const{ return _border_shadow; }
      virtual void border_shadow(const rgb& newval){
        _border_shadow = newval;
        refresh_border();
      }

      virtual border_styles border_style() const{ return _border_style; }
      virtual void border_style(border_styles newval){
        _border_style = newval;
        refresh_border();
      }

      virtual void enable_border_elements(bool top, bool right, bool bottom, bool left){
        _draw_top = top; _draw_left = left; _draw_right = right; _draw_bottom = bottom;
      }

    protected:

      explicit has_border(window * pParent) : _SuperT(pParent){}

      void refresh_border(){
        if (!_SuperT::_handle) return;
        wtf::exception::throw_lasterr_if(
          ::RedrawWindow(*this, nullptr, nullptr, RDW_FRAME | RDW_INVALIDATE | RDW_NOCHILDREN),
          [](BOOL b){ return !b; }
        );
      }

      void on_wm_ncpaint(const wtf::_::device_context& dc, const rect<coord_frame::client>& oClient) override{

        auto highlight = pen::create(pen::style::solid, 1, border_highlight());
        auto shadow = pen::create(pen::style::solid, 1, border_shadow());
        rect<coord_frame::client> client = oClient;
        client.right--;
        client.bottom--;
        wtf::_::effects::draw_border(dc, client, border_style(), highlight, shadow, _draw_left, _draw_top, _draw_right, _draw_bottom);

        return _SuperT::on_wm_ncpaint(dc, oClient);
      }

      LRESULT on_wm_nccalcsize(NCCALCSIZE_PARAMS * pSizes) override{
        pSizes->rgrc[0].top += border_width();
        pSizes->rgrc[0].left += border_width();
        pSizes->rgrc[0].bottom -= border_width();
        pSizes->rgrc[0].right -= border_width();
        return WVR_VALIDRECTS | WVR_REDRAW;
      }

    private:
      border_styles _border_style = border_styles::none;
      rgb _border_highlight = system_rgb<system_colors::button_highlight>();
      rgb _border_shadow = system_rgb<system_colors::button_shadow>();
      bool _draw_top = true;
      bool _draw_left = true;
      bool _draw_right = true;
      bool _draw_bottom = true;
    };

  }

  namespace _{
    template <> struct policy_traits<policy::has_border>{
      using requires = policy_list<policy::wm_ncpaint, policy::wm_nccalcsize>;
    };
  }
}