/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{
    template <typename _super_t>
    struct isa_label : _super_t{

    protected:
      explicit isa_label(window * pParent)  : _super_t(pParent){}
    };
  }


  struct label : window_impl<label, policy::isa_label >{
    explicit label(window * pParent)  : window_impl(pParent){}
  };

}
