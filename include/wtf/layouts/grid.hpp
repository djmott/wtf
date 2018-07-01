/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once
namespace wtf {
  namespace layouts {
    struct grid {

      struct cell {
        using pointer = std::shared_ptr<cell>;
        using vector = std::vector<pointer>;
        enum class locations {
          top,
          left,
          bottom,
          right,
          center,
        };

        locations location() const { return _location; }
        void location(locations newval) { _location = newval; }


        ~cell() = default;
        cell() = delete;
        cell(const cell&) = default;
        cell& operator=(const cell&) = default;

        pointer make(window * wnd, locations loc) { return pointer(new cell(wnd, loc)); }

      protected:
        cell(window * wnd, locations loc) : _window(wnd), _location(loc) {}
        window * _window;
        locations _location;
      };

      struct row {
        using pointer = std::shared_ptr<row>;
        using vector = std::vector<pointer>;
        enum class sizes {
          fill,
          fixed,
          none,
        };
        sizes size() const { return _size; }
        void size(sizes newval) { _size = newval; }
      protected:
        sizes _size;
      };

    };
  }
}
