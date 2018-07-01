/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#define DOXY_INHERIT_HAS_INVALIDATE

namespace wtf{
  namespace policy{

    /** @class has_invalidate
    @brief Behavior policy of a window that can be invalidated
    @ingroup Policies
    */
    template <typename _super_t>
    struct has_invalidate : _super_t{
      
      //! @brief invalidates the window causing it to be repainted
      //! @param[in] EraseBackground Set to true to also invalidate the background
      virtual void invalidate(bool EraseBackground = true)  {
        if (_super_t::_handle) {
          wtf::exception::throw_lasterr_if(
            ::InvalidateRect(_super_t::_handle, nullptr, (EraseBackground ? TRUE : FALSE)),
                                           [](BOOL b)noexcept { return !b; });
        }
      }

    };
  }
}
