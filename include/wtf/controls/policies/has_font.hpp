/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {

    namespace policy {
      template <typename _super_t>
      struct has_font : _super_t {

        template <typename ... _arg_ts> has_font(_arg_ts&&...args)  : _super_t(std::forward<_arg_ts>(args)...),
          _font(wtf::_::non_client_metrics::get().lfMessageFont), _hfont(_font.open()){}

        virtual const wtf::font& font() const noexcept { return _font; }
        virtual void font(const wtf::font& newval) noexcept { 
          _font = newval;
          _hfont = _font.open();
          ::SendMessage(*this, WM_SETFONT, reinterpret_cast<WPARAM>(static_cast<HFONT>(_hfont)), TRUE);
        }

      protected:

        void on_created() override {
          ::SendMessage(*this, WM_SETFONT, reinterpret_cast<WPARAM>(static_cast<HFONT>(_hfont)), TRUE);
          _super_t::on_created();
        }

        wtf::font _font;
        wtf::font::handle _hfont;
      };
    }
  }
}