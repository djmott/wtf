/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT>
    struct isa_menu : _SuperT{

      static const DWORD ExStyle = WS_EX_NOPARENTNOTIFY;
      static const DWORD Style = WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_TABSTOP;

      void popup(int x, int y){
        _SuperT::move(x, y, 100, 100);
      }

    protected:

      explicit isa_menu(iwindow * pParent) : _SuperT(pParent){
        _SuperT::border_style(border_styles::double_raised);
      }
      
      int exec() override{ return 0; }

    };
  }

  namespace _{
    template <> struct policy_traits<policy::isa_menu>{
      using requires = policy_list<policy::isa_panel>;
    };

  }


  struct menu : window_impl<menu, policy::isa_menu>{
    explicit menu(iwindow * pParent) : window_impl(pParent){}
  };

}
