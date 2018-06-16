/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {

    namespace policy {
      template <typename _super_t>
      struct has_enable: _super_t {

        bool enabled() const { return !_super_t::get_style<WS_DISABLED>(); }

        void enabled(bool newval) { _super_t::set_style<WS_DISABLED>(!newval); }

      };
    }
  }
}