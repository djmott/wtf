#pragma once

#if !defined(DCX_NODELETERGN ) 
#define DCX_NODELETERGN 0x00040000
#endif
#if !defined(DCX_USESTYLE)
#define DCX_USESTYLE 0x00010000
#endif

namespace wtf {

  namespace policy {
    /** has_border
    * Creates borders
    */
    template<typename _SuperT, typename _ImplT>
    struct has_border : _SuperT::window_type::template add_policy<messages::wm_ncpaint, messages::wm_nccalcsize> {

      enum class border_styles{
        none = 0,
        flat,
        raised,
        lowered,
        bumped,
        etched,
        double_raised,
        double_lowered,
      };

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
      has_border(window<void> * pParent) : _SuperT(pParent){}

      void refresh_border(){
        if (!_handle) return;
        wtf::exception::throw_lasterr_if(
          ::RedrawWindow(*this, nullptr, nullptr, RDW_FRAME | RDW_INVALIDATE | RDW_NOCHILDREN),
          [](BOOL b){ return !b; }
        );
      }



      virtual void wm_ncpaint(const device_context& dc, rect<coord_frame::client>& oClient){
/*

        auto redBrush = brush::solid_brush(rgb(255, 0, 0));

        dc.fill(oClient, redBrush);
        return;


*/


        auto highlight = pen::create(pen::style::solid, 1, border_highlight());
        auto shadow = pen::create(pen::style::solid, 1, border_shadow());

        rect<coord_frame::client> client = oClient;
/*
        client.bottom += border_width()-1;
        client.right += border_width()-1;
        */
        client.bottom--;
        client.right--;



        //draw outer border
        switch (border_style()){
          case border_styles::none:
            return;
          case border_styles::flat:
            if (_draw_right) dc.line(shadow, client.right, client.top, client.right, 1 + client.bottom);
            if (_draw_bottom) dc.line(shadow, client.left, client.bottom, client.right, client.bottom);
            if (_draw_top) dc.line(shadow, client.left, client.top, client.right, client.top);
            if (_draw_left) dc.line(shadow, client.left, client.top, client.left, client.bottom);
            return;
          case border_styles::etched:
          case border_styles::lowered:
          case border_styles::double_lowered:
            std::swap(highlight, shadow); //fall through
          case border_styles::bumped:
          case border_styles::raised:
          case border_styles::double_raised:
            if (_draw_right) dc.line(shadow, client.right, client.top, client.right, 1 + client.bottom);
            if (_draw_bottom) dc.line(shadow, client.left, client.bottom, client.right, client.bottom);
            if (_draw_top) dc.line(highlight, client.left, client.top, client.right, client.top);
            if (_draw_left) dc.line(highlight, client.left, client.top, client.left, client.bottom);
        }
        //draw inner border
        client.left++;
        client.right--;
        client.top++;
        client.bottom--;
        switch (border_style()){
          case border_styles::raised:
          case border_styles::lowered:
            return;
          case border_styles::etched:
          case border_styles::bumped:
            std::swap(highlight, shadow);
          case border_styles::double_raised:
          case border_styles::double_lowered:
            if (_draw_right) dc.line(shadow, client.right, client.top, client.right, 1 + client.bottom);
            if (_draw_bottom) dc.line(shadow, client.left, client.bottom, client.right, client.bottom);
            if (_draw_top) dc.line(highlight, client.left, client.top, client.right, client.top);
            if (_draw_left) dc.line(highlight, client.left, client.top, client.left, client.bottom);
        }

/*
        auto red = pen::create(pen::style::solid, 1, rgb(255, 0, 0));
        auto black = pen::create(pen::style::solid, 1, rgb(0, 0, 0));
        dc.line(black, 0, 0, oClient.right, oClient.bottom);
        dc.line(black, oClient.left, oClient.bottom, oClient.right, oClient.top);
        dc.line(red, client.left, client.top, client.right, client.bottom);
        dc.line(red, client.left, client.bottom, client.right, client.top);
*/

      }
      virtual LRESULT on_wm_nccalcsize(NCCALCSIZE_PARAMS * pSizes, bool& bHandled) override{
        bHandled = true;
        pSizes->rgrc[0].top += border_width();
        pSizes->rgrc[0].left += border_width();
        pSizes->rgrc[0].bottom -= border_width();
        pSizes->rgrc[0].right -= border_width();
        return WVR_VALIDRECTS | WVR_REDRAW; */
      }
      virtual LRESULT on_wm_nccalcsize(RECT * pClient, bool& bHandled) override{
        bHandled = true;
      }

      LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool &handled) {
        return 0;
        if (WM_NCCALCSIZE == umsg && wparam){
/*
          auto pSizeParam = reinterpret_cast<NCCALCSIZE_PARAMS*>(lparam);
          pSizeParam->rgrc[0].top += border_width();
          pSizeParam->rgrc[0].left += border_width();
          pSizeParam->rgrc[0].bottom -= border_width();
          pSizeParam->rgrc[0].right -= border_width();
          handled = true;
          return WVR_VALIDRECTS| WVR_REDRAW;*/
        }else if (WM_NCCALCSIZE == umsg){
/*
          auto oClient = reinterpret_cast<RECT*>(lparam);
          oClient->bottom = oClient->bottom;
*/
        } else if (WM_NCPAINT == umsg){
/*
          handled = true;
          if (1 == wparam){
            auto oDC = device_context::get_dcex(*this, DCX_WINDOW | DCX_USESTYLE| DCX_CLIPSIBLINGS | DCX_CLIPCHILDREN);
            auto oWindow = rect<coord_frame::screen>::get(*this);


            oWindow.offset(oWindow.position());
            rect<coord_frame::client> oClient(oWindow);
            wm_ncpaint(oDC, oClient);

          } else{

            auto oWindow = rect<coord_frame::screen>::get(*this);

            auto oRegion = region::attach((HRGN)wparam);
            oRegion.offset(oWindow.position());

            auto oDC = device_context::get_dcex(*this, oRegion, DCX_EXCLUDERGN | DCX_WINDOW | DCX_USESTYLE | DCX_CLIPSIBLINGS | DCX_CLIPCHILDREN);

            oWindow.offset(oWindow.position());
            rect<coord_frame::client> oClient(oWindow);
            wm_ncpaint(oDC, oClient);
          }
          */

        }

/* WORKS
        } else if (WM_NCPAINT == umsg){
          handled = true;
          auto oDC = device_context::get_dcex(*this, DCX_WINDOW | DCX_CACHE | DCX_USESTYLE);
          auto oWindow = rect<coord_frame::screen>::get(*this);
          if (1 == wparam){
            oWindow.offset(oWindow.position());
            oDC.intersect_clip_rect(oWindow);
          } else{
            auto oRegion = region::attach((HRGN)wparam);
            oRegion.offset(oWindow.position());
            oDC.select_clip_rgn(oRegion);
            oWindow.offset(oWindow.position());
          }
          rect<coord_frame::client> oClient(oWindow);
          wm_ncpaint(oDC, oClient);
        }
*/

/*
this somewhat works
        } else if (WM_NCPAINT == umsg){
          handled = true;
          auto oDC = device_context::get_dcex(*this, DCX_WINDOW | DCX_CACHE | DCX_USESTYLE);
          auto oWindow = rect<coord_frame::screen>::get(*this);
          if (1==wparam ){
            oWindow.offset(oWindow.position());
            oDC.intersect_clip_rect(oWindow);
          }else{
            auto oRegion = region::attach((HRGN)wparam);
            oRegion.offset(oWindow.position());
            oDC.select_clip_rgn(oRegion);
            oWindow.offset(oWindow.position());
          }
          rect<coord_frame::client> oClient(oWindow);
          wm_ncpaint(oDC, oClient);
        }
*/
        return 0;
      }

    private:
      border_styles _border_style = border_styles::raised;
      rgb _border_highlight = system_rgb<system_colors::button_highlight>();
      rgb _border_shadow = system_rgb<system_colors::button_shadow>();
      bool _draw_top = true;
      bool _draw_left = true;
      bool _draw_right = true;
      bool _draw_bottom = true;
    };

  }
}
