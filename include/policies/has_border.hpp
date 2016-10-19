#pragma once

namespace wtf {

  namespace policy {
    /** has_border
    * Creates borders
    */
    template<typename _SuperT, typename _ImplT>
    struct has_border : _SuperT {

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
      virtual void border_highlight(const rgb& newval){ _border_highlight = newval;  }

      virtual const rgb& border_shadow() const{ return _border_shadow; }
      virtual void border_shadow(const rgb& newval){ _border_shadow = newval; }

      virtual border_styles border_style() const{ return _border_style; }
      virtual void border_style(border_styles newval){ _border_style = newval; }

      virtual void enable_border_elements(bool top, bool right, bool bottom, bool left){
        _draw_top = top; _draw_left = left; _draw_right = right; _draw_bottom = bottom;
      }
    protected:
      has_border(iwindow * pParent) : _SuperT(pParent){}

      virtual void draw_border(const device_context& dc, const rect::client_coord & oClient){
        auto highlight = pen::create(pen::style::solid, 1, border_highlight());
        auto shadow = pen::create(pen::style::solid, 1, border_shadow());
        rect::client_coord client = oClient;
        client.bottom--;
        client.right--;
        //draw outer border
        switch (border_style()){
          case border_styles::none:
            return;
          case border_styles::flat:
            if (_draw_right) dc.line(shadow, client.right, client.top, client.right, client.bottom);
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

      }

      virtual void DrawBorderEvent(const device_context& dc, rect::client_coord& oClient){
        draw_border(dc, oClient);
      }

      LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool &handled) {
        if (WM_NCCALCSIZE == umsg && wparam){
          auto pSizeParam = reinterpret_cast<NCCALCSIZE_PARAMS*>(lparam);
          handled = true;
          pSizeParam->rgrc[0].top += border_width();
          pSizeParam->rgrc[0].left += border_width();
          pSizeParam->rgrc[0].bottom -= border_width();
          pSizeParam->rgrc[0].right -= border_width();

        } else if (WM_NCPAINT == umsg){
          handled = true;
          auto oClient = rect::client_coord::get(*this);
          oClient.bottom += border_width() * 2;
          oClient.right += border_width() * 2;
          DrawBorderEvent(device_context::get_window(*this), oClient);
        }
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
