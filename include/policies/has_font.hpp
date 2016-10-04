#pragma once

namespace wtf {
  namespace policy {
    template<typename _SuperT>
    struct has_font : _SuperT {

      has_font() : _SuperT(),
                   _font(_::non_client_metrics::get().lfMessageFont),
                   _fore_color(system_rgb<system_colors::window_text>()),
                   _back_color(system_rgb<system_colors::window>()),
                   _background_mix_mode(background_mix_modes::transparent) {}

      has_font(const has_font&) = delete;
      has_font &operator=(const has_font &) = delete;
      has_font(has_font&&) = delete;
      has_font &operator=(has_font&&) = delete;

      using background_mix_modes = device_context::background_mix_modes;

      virtual background_mix_modes background_mix_mode() const { return _background_mix_mode; }

      void background_mix_mode(background_mix_modes newval) { _background_mix_mode = newval; }

      virtual const wtf::font &font() const { return _font; }

      virtual wtf::font &font() { return _font; }

      void font(wtf::font &newval) { _font = newval; }

      virtual rgb fore_color() const { return _fore_color; }

      void fore_color(rgb newval) { _fore_color = newval; }

      virtual rgb back_color() const { return _back_color; }

      void back_color(rgb newval) { _back_color = newval; }

    protected:
      wtf::font _font;
      rgb _fore_color;
      rgb _back_color;
      background_mix_modes _background_mix_mode;


    };
  }
}
