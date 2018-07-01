/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {

  namespace policy {
    template <typename _super_t>
    struct has_hscroll : _super_t {

      virtual bool hscroll() const { return _super_t::get_style_bit<WS_HSCROLL>(); }

      virtual void hscroll(bool newval) { _super_t::set_style_bit<WS_HSCROLL>(newval); }

    };
  }
}