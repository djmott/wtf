/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once
namespace wtf{

  enum class orientations{
    horizontal,
    vertical
  };

  namespace policy{

    template <typename _super_t>
    struct has_orientation : _super_t{
      
      virtual orientations orientation() const { return _orientation; }
      virtual void orientation(orientations newval) { _orientation = newval; }

      has_orientation() : _super_t() {}

    protected:
      orientations _orientation = orientations::horizontal;
    };
  }
}
