/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace policy {

    /** @class has_font
    Behavior policy of font settings
    @ingroup Policies
    */
    template <typename _super_t>
    struct has_font : _super_t {

      has_font() : _super_t(), _font(wtf::_::non_client_metrics::get().lfMessageFont), _hfont(_font.open()) {}

      virtual const wtf::font& font() const noexcept { return _font; }

      virtual void font(const wtf::font& newval) noexcept {
        _font = newval;
        _hfont = _font.open();
        ::SendMessage(*this, WM_SETFONT, reinterpret_cast<WPARAM>(static_cast<HFONT>(_hfont)), TRUE);
      }

    protected:
      template <typename, template <typename> typename...> friend struct window_impl;

      void on_created() override {
        ::SendMessage(*this, WM_SETFONT, reinterpret_cast<WPARAM>(static_cast<HFONT>(_hfont)), TRUE);
        _super_t::on_created();
      }

      wtf::font _font;
      wtf::font::handle _hfont;
    };
  }
}