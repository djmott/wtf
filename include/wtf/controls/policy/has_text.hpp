/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {

    namespace policy {
      template <typename _super_t>
      struct has_text : _super_t {

        template <typename ... _arg_ts> has_text(_arg_ts&&...args) noexcept : _super_t(std::forward<_arg_ts>(args)...) {}

        void text(const tstring& newval) {
          wtf::exception::throw_lasterr_if(SetWindowText(*this, newval.c_str()), [](BOOL b) { return !b; });
        }

        tstring text() const {
          tstring ret(GetWindowTextLength(*this), 0);
          GetWindowText(*this, &ret[0], ret.size());
          return ret;
        }

      };
    }
  }
}