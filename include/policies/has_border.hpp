#pragma once

namespace wtf {

  namespace policy {
    /** has_border
    * Creates borders
    */
    template<typename _SuperT>
    struct has_border : _SuperT {

      virtual ~has_border() = default;

      has_border() :
        _SuperT(),
        _border_style(border_styles::raised),
        _border_highlight(system_rgb<system_colors::button_highlight>()),
        _border_shadow(system_rgb<system_colors::button_shadow>())
      {}

      has_border(const has_border&) = delete;
      has_border(has_border&&) = delete;
      has_border &operator=(const has_border &) = delete;
      has_border &operator=(has_border&&) = delete;

      enum class border_styles{
        none = 0,
        flat,
        raised,
        lowered,
        bumped,
        etched,
      };

      int border_width() const{
        switch (_border_style){
          case border_styles::none: return 0;
          case border_styles::flat:
          case border_styles::raised:
          case border_styles::lowered: return 1;
          default: return 2;
        }
      }
      virtual rgb border_highlight() const{ return _border_highlight; }
      virtual void border_highlight(rgb newval){ _border_highlight = newval; }

      virtual rgb border_shadow() const{ return _border_shadow; }
      virtual void border_shadow(rgb newval){ _border_shadow = newval; }

      virtual border_styles border_style() const{ return _border_style; }
      virtual void border_style(border_styles newval){ _border_style = newval; }

    protected:

      virtual void DrawBorderEvent(const device_context& dc, const paint_struct& ps){
        auto highlight = pen::create(pen::style::solid, 1, border_highlight());
        auto shadow = pen::create(pen::style::solid, 1, border_shadow());
        //draw outer border
        switch (border_style()){
          case border_styles::none: return;
          case border_styles::flat:
            dc.line(shadow, ps.rcPaint.right - 1, 0, ps.rcPaint.right - 1, ps.rcPaint.bottom);
            dc.line(shadow, 0, ps.rcPaint.bottom-1, ps.rcPaint.right, ps.rcPaint.bottom-1);
            dc.line(shadow, 0, 0, ps.rcPaint.right, 0);
            dc.line(shadow, 0, 0, 0, ps.rcPaint.bottom);
            break;
          case border_styles::etched:
          case border_styles::lowered:
            std::swap(highlight, shadow);
          case border_styles::bumped:
          case border_styles::raised:
            dc.line(shadow, ps.rcPaint.right - 1, 0, ps.rcPaint.right - 1, ps.rcPaint.bottom );
            dc.line(shadow, 0, ps.rcPaint.bottom - 1, ps.rcPaint.right, ps.rcPaint.bottom -1);
            dc.line(highlight, 0, 0, ps.rcPaint.right, 0);
            dc.line(highlight, 0, 0, 0, ps.rcPaint.bottom);
        }
        //draw inner border
        switch (border_style()){
          case border_styles::raised:
          case border_styles::lowered: return;
          case border_styles::etched:
            std::swap(highlight, shadow);
          case border_styles::bumped:
            dc.line(shadow, ps.rcPaint.right - 2, 1, ps.rcPaint.right - 2, ps.rcPaint.bottom - 2);
            dc.line(shadow, 1, ps.rcPaint.bottom - 2, ps.rcPaint.right - 2, ps.rcPaint.bottom - 2);
            dc.line(highlight, 1, 1, ps.rcPaint.right-1, 1);
            dc.line(highlight, 1, 1, 1, ps.rcPaint.bottom-1);
        }
      }

      virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool &bProcessed) override {
        if (WM_PAINT == umsg ) {
          DrawBorderEvent(*reinterpret_cast<const device_context *>(wparam), *reinterpret_cast<const paint_struct *>(lparam));
        } 
        return 0;
      }

    private:
      border_styles _border_style;
      rgb _border_highlight;
      rgb _border_shadow;
    };

  }
}
