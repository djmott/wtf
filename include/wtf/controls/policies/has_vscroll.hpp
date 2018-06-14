/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {

    namespace policy {
      template <typename _super_t>
      struct has_vscroll : _super_t {

        bool vscroll() const { return get_style<WS_VSCROLL>(); }

        void vscroll(bool newval) { set_style<WS_VSCROLL>(newval); }

      };
    }
  }
}