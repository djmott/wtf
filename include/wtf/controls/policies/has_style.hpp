/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {

    namespace policy {
      template <typename _super_t>
      struct has_style: _super_t {

        has_style() : _super_t() {}

      protected:
        template <DWORD _style_flag>
        bool get_style() const {
          return _style_flag & wtf::exception::throw_lasterr_if(::GetWindowLong(*this, GWL_STYLE), [](LONG l) { return !l; });
        }

        template <DWORD _style_flag>
        void set_style(bool newval) {
          auto style = wtf::exception::throw_lasterr_if(::GetWindowLong(*this, GWL_STYLE), [](LONG l) { return !l; });
          style &= ~_style_flag;
          if (newval) style |= _style_flag;
          wtf::exception::throw_lasterr_if(::SetWindowLong(*this, GWL_STYLE, style), [](LONG l) { return !l; });
        }

      };
    }
  }
}