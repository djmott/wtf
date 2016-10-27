#pragma once

namespace wtf {

  enum class font_background_modes{
    opaque = OPAQUE,
    transparent = TRANSPARENT,
  };


    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::has_font, _Policies...> 
      : public window_impl<_ImplT, _Policies..., policy::wm_create, policy::wm_paint>{
      using __super_t = window_impl<_ImplT, _Policies..., policy::wm_create, policy::wm_paint>;
      template <typename, policy ... > friend class window_impl;
    public:

      virtual font_background_modes background_mode() const { return _background_mode; }
      virtual void background_mode(font_background_modes newval) { _background_mode = newval; }

      virtual const wtf::font& font() const { return _font; }
      virtual wtf::font& font() { return _font; }
      virtual void font(const wtf::font& newval) { _font = newval; }

      virtual rgb fore_color() const { return _fore_color; }
      virtual void fore_color(rgb newval) { _fore_color = newval; }

      virtual rgb back_color() const { return _back_color; }
      virtual void back_color(rgb newval) { _back_color = newval; }

    protected:
      
      explicit window(iwindow * pParent) : __super_t(pParent){}

      virtual void on_wm_create() override{
        apply_font(device_context::get_client(*this));
        return __super_t::on_wm_create();
      }

      virtual void on_wm_paint(const device_context& dc, const paint_struct& ps) override{
        apply_font(dc);
        return __super_t::on_wm_paint(dc, ps);
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
