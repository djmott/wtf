#pragma once

namespace wtf {

  enum class font_background_modes{
    opaque = OPAQUE,
    transparent = TRANSPARENT,
  };

  namespace policy {

    template<typename _SuperT, typename _ImplT>
    struct has_font : _SuperT {

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
      
      explicit has_font(window<void,void> * pParent) : _SuperT(pParent){}

      virtual LRESULT on_wm_create(bool& bHandled) override{
        apply_font(device_context::get_client(*this));
        return _SuperT::on_wm_create(bHandled);
      }

      virtual LRESULT on_wm_paint(const device_context& dc, const paint_struct& ps, bool& bHandled) override{
        apply_font(dc);
        return _SuperT::on_wm_paint(dc, ps, bHandled);
      }

      virtual LRESULT on_wm_erasebkgnd(const device_context& dc, const rect<coord_frame::client>& client, bool& bHandled) override{ 
        apply_font(dc);
        return _SuperT::on_wm_erasebkgnd(dc, client, bHandled);
      }

      void apply_font(const device_context& dc){
        dc.select_object(font().open());
        wtf::exception::throw_lasterr_if(::SetBkMode(dc, static_cast<int>(background_mode())), [](int i){ return !i; });
        wtf::exception::throw_lasterr_if(::SetTextColor(dc, fore_color()), [](COLORREF c){ return CLR_INVALID == c; });
        wtf::exception::throw_lasterr_if(::SetBkColor(dc, back_color()), [](COLORREF c){ return CLR_INVALID == c; });
      }
    
    private:

      wtf::font _font = _::non_client_metrics::get().lfMessageFont;
      rgb _fore_color = system_rgb<system_colors::window_text>();
      rgb _back_color = system_rgb<system_colors::button_face>();
      font_background_modes _background_mode = font_background_modes::transparent;
    };
  }
}
