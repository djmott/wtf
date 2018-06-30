/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once
namespace wtf {
  namespace layout {
    struct grid {

      struct cell {
        enum class locations {
          top,
          left,
          bottom,
          right,
          fill,
        };

        locations location() const { return _location; }
        void location(locations newval) { _location = newval; }

        ~cell() = default;
        cell() = delete;
        cell(const cell&) = default;
        cell& operator=(const cell&) = default;
        cell(window * wnd, locations loc) : _window(wnd), _location(loc){}

      private:
        window * _window;
        locations _location;
      };

      struct row {

      };

    };
  }
}
