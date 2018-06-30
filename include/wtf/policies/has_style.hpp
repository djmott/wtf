/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#define DOXY_INHERIT_HAS_STYLE

namespace wtf {

  namespace policy {

    /** @internal
    @class has_style
    @brief Behavior policy that gets and sets window style bits
    @ingroup Policies
    @endinternal
    */

    template <typename _super_t>
    struct has_style : _super_t {

    protected:
      //! @brief Gets a window style value
      LONG_PTR get_style_value() const {
        return wtf::exception::throw_lasterr_if(::GetWindowLongPtr(*this, GWL_STYLE), [](LONG_PTR l) { return !l; });
      }

      //! @brief Gets a window style bits
      template <DWORD _style_flag>
      bool get_style_bit() const {
        return _style_flag & get_style_value();
      }

      //! @brief Sets a window style value
      void set_style_value(LONG_PTR newval) {
        wtf::exception::throw_lasterr_if(::SetWindowLongPtr(*this, GWL_STYLE, newval), [](LONG_PTR l) { return !l; });
      }
      //! @brief Sets a window style bits
      template <DWORD _style_flag>
      void set_style_bit(bool newval) {
        auto style = (get_style_value() & ~_style_flag);
        if (newval) style |= _style_flag;
        set_style_value(style);
      }

    };
  }
}