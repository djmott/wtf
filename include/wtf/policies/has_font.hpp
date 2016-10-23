#pragma once

namespace wtf {

  enum class font_background_modes{
    opaque = OPAQUE,
    transparent = TRANSPARENT,
  };

  namespace policy {
    template<typename _SuperT, typename _ImplT>
    struct has_font : _SuperT {

      has_font() : _SuperT(), _font(_::non_client_metrics::get().lfMessageFont){}

      has_font(const has_font&) = delete;
      has_font &operator=(const has_font &) = delete;
      has_font(has_font&&) = delete;
      has_font &operator=(has_font&&) = delete;


      virtual font_background_modes background_mode() const { return _background_mode; }
      virtual void background_mode(font_background_modes newval) { _background_mode = newval; }

      virtual const wtf::font& font() const { return _font; }
      virtual wtf::font &font() { return _font; }
      virtual void font(const wtf::font& newval) { _font = newval; }

      virtual rgb fore_color() const { return _fore_color; }
      virtual void fore_color(rgb newval) { _fore_color = newval; }

      virtual rgb back_color() const { return _back_color; }
      virtual void back_color(rgb newval) { _back_color = newval; }

    protected:
      has_font(window<void> * pParent) : _SuperT(pParent){}

      virtual void ApplyFontEvent(const device_context& dc){
        const auto & oFont = font();
        auto oFontHandle = oFont.open();
        dc.select_object(oFontHandle);
        wtf::exception::throw_lasterr_if(::SetBkMode(dc, static_cast<int>(background_mode())), [](int i){ return !i; });
        wtf::exception::throw_lasterr_if(::SetTextColor(dc, fore_color()), [](COLORREF c){ return CLR_INVALID == c; });
        wtf::exception::throw_lasterr_if(::SetBkColor(dc, back_color()), [](COLORREF c){ return CLR_INVALID == c; });
      }

      LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool &bProcessed) {
        if (WM_PAINT == umsg){
          ApplyFontEvent(*reinterpret_cast<const device_context *>(wparam));
        }
        return 0;
      }
    
    private:


      wtf::font _font = _::non_client_metrics::get().lfMessageFont;
      rgb _fore_color = system_rgb<system_colors::window_text>();
      rgb _back_color = system_rgb<system_colors::window>();
      font_background_modes _background_mode = font_background_modes::transparent;


    };
  }
}
