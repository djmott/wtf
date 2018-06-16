/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {

  template <coord_frame _frame> struct point : POINT{
    using vector = std::vector<point>;
    point() noexcept { x = y = 0; }
    point(LONG X, LONG Y) noexcept { x = X; y = Y; }
  };
}

