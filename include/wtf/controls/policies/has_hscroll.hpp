﻿/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {

    namespace policy {
      template <typename _super_t>
      struct has_hscroll : _super_t {

        has_hscroll() : _super_t() {}

        bool hscroll() const { return _super_t::get_style<WS_HSCROLL>(); }

        void hscroll(bool newval) { _super_t::set_style<WS_HSCROLL>(newval); }

      };
    }
  }
}