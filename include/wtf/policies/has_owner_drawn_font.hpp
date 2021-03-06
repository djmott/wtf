﻿/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  enum class font_background_modes{
    opaque = OPAQUE,
    transparent = TRANSPARENT,
  };

  namespace policy{

    template <typename _super_t>
    struct has_owner_drawn_font : _super_t{

      virtual font_background_modes background_mode() const noexcept { return _background_mode; }
      virtual void background_mode(font_background_modes newval) noexcept { _background_mode = newval; _super_t::invalidate(); }

      virtual const wtf::font& font() const noexcept { return _font; }
      virtual wtf::font& font() noexcept { return _font; }
      virtual void font(const wtf::font& newval) noexcept { _font = newval; _super_t::invalidate(); }

      virtual rgb fore_color() const noexcept { return _fore_color; }
      virtual void fore_color(rgb newval) noexcept { _fore_color = newval; _super_t::invalidate(); }

      virtual rgb back_color() const noexcept { return _back_color; }
      virtual void back_color(rgb newval) noexcept { _back_color = newval; _super_t::invalidate(); }

    protected:
      
      void on_wm_erasebkgnd(const device_context& dc, const rect<coord_frame::client>& client) override {
        apply_font(dc);
        return _super_t::on_wm_erasebkgnd(dc, client);
      }

      void apply_font(const device_context& dc)  {
        dc.select_object(font().open());
        wtf::exception::throw_lasterr_if(::SetBkMode(dc, static_cast<int>(background_mode())), [](int i)noexcept { return !i; });
        wtf::exception::throw_lasterr_if(::SetTextColor(dc, fore_color()), [](COLORREF c)noexcept { return CLR_INVALID == c; });
        wtf::exception::throw_lasterr_if(::SetBkColor(dc, back_color()), [](COLORREF c)noexcept { return CLR_INVALID == c; });
      }

    private:

      wtf::font _font = wtf::_::non_client_metrics::get().lfMessageFont;
      rgb _fore_color = system_rgb<system_colors::window_text>();
      rgb _back_color = system_rgb<system_colors::button_face>();
      font_background_modes _background_mode = font_background_modes::transparent;
    };
  }


}
