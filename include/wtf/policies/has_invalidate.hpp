/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{

    template <typename _super_t>
    struct has_invalidate : _super_t{
      
      void invalidate(bool EraseBackground = true)  {
        if (_super_t::_handle) {
          wtf::exception::throw_lasterr_if(
            ::InvalidateRect(_super_t::_handle, nullptr, (EraseBackground ? TRUE : FALSE)),
                                           [](BOOL b)noexcept { return !b; });
        }
      }

    protected:
      explicit has_invalidate(window * pParent) noexcept : _super_t(pParent){}

    };
  }
}
