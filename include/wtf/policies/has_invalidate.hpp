/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{

    template <typename _SuperT>
    struct has_invalidate : _SuperT{
      
      void invalidate(bool EraseBackground = true){
        if (_SuperT::_handle) {
          wtf::exception::throw_lasterr_if(
            ::InvalidateRect(_SuperT::_handle, nullptr, (EraseBackground ? TRUE : FALSE)),
                                           [](BOOL b){ return !b; });
        }
      }

    protected:
      explicit has_invalidate(window * pParent) : _SuperT(pParent){}

    };
  }
}
