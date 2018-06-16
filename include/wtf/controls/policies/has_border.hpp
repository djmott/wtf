/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {

    enum class border_styles {
      none,
      single,
      thick,
      thin,
    };

    namespace policy {
      /**
       * @class has_border
       * Behavior policy of a widget or form with a border
       * @ingroup Policies
       */
      template <typename _super_t>
      struct has_border: _super_t {

        has_border() : _super_t(){}

        /**
         * @fn void border(border_styles newval)
         * Sets the border style
         */
        void border(border_styles newval) {
          auto style = wtf::exception::throw_lasterr_if(::GetWindowLong(*this, GWL_STYLE), [](LONG l) { return !l; });
          auto exstyle = wtf::exception::throw_lasterr_if(::GetWindowLong(*this, GWL_EXSTYLE), [](LONG l) { return !l; });
          style &= ~WS_BORDER;
          exstyle &= ~WS_EX_CLIENTEDGE;
          exstyle &= ~WS_EX_STATICEDGE;
          switch (newval) {
            case border_styles::single: style |= WS_BORDER; break;
            case border_styles::thick: exstyle |= WS_EX_CLIENTEDGE; break;
            case border_styles::thin: exstyle |= WS_EX_STATICEDGE; break;
            default: break;
          }
          
          wtf::exception::throw_lasterr_if(::SetWindowLong(*this, GWL_STYLE, style), [](LONG l) { return !l; });
          wtf::exception::throw_lasterr_if(::SetWindowLong(*this, GWL_EXSTYLE, exstyle), [](LONG l) { return !l; });
        }

        /**
         * @fn border_styles border()
         * Gets the border style
         */
        border_styles border() const {
          auto style = wtf::exception::throw_lasterr_if(::GetWindowLong(*this, GWL_STYLE), [](LONG l) { return !l; });
          auto exstyle = wtf::exception::throw_lasterr_if(::GetWindowLong(*this, GWL_EXSTYLE), [](LONG l) { return !l; });
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
}