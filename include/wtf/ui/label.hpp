/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT>
    struct isa_label : _SuperT{

    protected:
      explicit isa_label(window * pParent) : _SuperT(pParent){}
    };
  }

  namespace _{

    template <> struct policy_traits<policy::isa_label>{
      using requires = policy_list<policy::isa_panel, policy::has_text>;
    };

  }

  struct label : window_impl<label, policy::isa_label >{
    explicit label(window * pParent) : window_impl(pParent){}
  };

}
