/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#define DOXY_INHERIT_HAS_TEXT

namespace wtf {

  namespace policy {

    /** @class has_text
    @brief Behavior policy of a window that contains printable text
    @ingroup Policies
    */
    template <typename _super_t>
    struct has_text : _super_t {

      //! @brief Sets the window text
      void text(const tstring& newval) {
        wtf::exception::throw_lasterr_if(SetWindowText(*this, newval.c_str()), [](BOOL b) { return !b; });
      }

      //! @brief Gets the window text
      tstring text() const {
        tstring ret(GetWindowTextLength(*this), 0);
        GetWindowText(*this, &ret[0], ret.size());
        return ret;
      }

    };
  }
}