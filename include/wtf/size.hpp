/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {

    struct size : SIZE {
      size() { cx = cy = 0; }

      size(int x, int y) {
        cx = x;
        cy = y;
      }
    };
  }

