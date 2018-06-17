/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{

    /** has_close
    * Add close method
    */
    template <typename _super_t>
    struct has_close :  _super_t{

      virtual void close() noexcept { CloseWindow(*this); }


    };
  }
}