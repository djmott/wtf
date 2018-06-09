/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#if 0

namespace wtf{

  namespace policy{
    template <typename _super_t>
    struct isa_menu : _super_t{

      static const DWORD ExStyle = WS_EX_NOPARENTNOTIFY;
      static const DWORD Style = WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_TABSTOP;

      void popup(int x, int y){
        _super_t::move(x, y, 100, 100);
      }

    protected:

      explicit isa_menu(window * pParent) : _super_t(pParent){
        _super_t::border_style(border_styles::double_raised);
      }
      
//       void exec() override{ return 0; }

    };
  }

  namespace _{
    template <> struct policy_traits<policy::isa_menu>{
      using requires = policy_list<policy::isa_panel>;
    };

  }


  struct menu : window_impl<menu, policy::isa_menu>{
    explicit menu(window * pParent) noexcept : window_impl(pParent){}
  };

}
#endif