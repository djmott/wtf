/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#define DOXY_INHERIT_HAS_BORDER

namespace wtf {

    enum class border_styles {
      none,
      single,
      thick,
      thin,
    };

    namespace policy {
      /** @class has_border
      @brief Behavior policy of a widget or form with a border
      @ingroup Policies
      */
      template <typename _super_t>
      struct has_border: _super_t {

        /** 
        @brief Sets the border style
        @param[in] newval new border style
         */
        void border(border_styles newval) {
          auto style = wtf::exception::throw_lasterr_if(::GetWindowLongPtr(*this, GWL_STYLE), [](LONG_PTR l) { return !l; });
          auto exstyle = wtf::exception::throw_lasterr_if(::GetWindowLongPtr(*this, GWL_EXSTYLE), [](LONG_PTR l) { return !l; });
          style &= ~WS_BORDER;
          exstyle &= ~WS_EX_CLIENTEDGE;
          exstyle &= ~WS_EX_STATICEDGE;
          switch (newval) {
            case border_styles::single: style |= WS_BORDER; break;
            case border_styles::thick: exstyle |= WS_EX_CLIENTEDGE; break;
            case border_styles::thin: exstyle |= WS_EX_STATICEDGE; break;
            default: break;
          }
          
          wtf::exception::throw_lasterr_if(::SetWindowLongPtr(*this, GWL_STYLE, style), [](LONG_PTR l) { return !l; });
          wtf::exception::throw_lasterr_if(::SetWindowLongPtr(*this, GWL_EXSTYLE, exstyle), [](LONG_PTR l) { return !l; });
          ::SetWindowPos(*this, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
        }

        /**
        @brief Gets the border style
        */
        border_styles border() const {
          auto style = wtf::exception::throw_lasterr_if(::GetWindowLongPtr(*this, GWL_STYLE), [](LONG_PTR l) { return !l; });
          auto exstyle = wtf::exception::throw_lasterr_if(::GetWindowLongPtr(*this, GWL_EXSTYLE), [](LONG_PTR l) { return !l; });
          if (style & WS_BORDER) return border_styles::single;
          if (style & WS_EX_CLIENTEDGE) return border_styles::thick;
          if (style & WS_EX_STATICEDGE) return border_styles::thin;
          return border_styles::none;
        }

      protected:
        template <typename, template <typename> typename...> friend struct window_impl;

      };
    }
}