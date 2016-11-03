/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once


namespace wtf{
  namespace policy{

    template <typename _SuperT>
    struct isa_toggle_button : _SuperT{

      bool value() const{ return _value; }
      void value(bool newval){
        if (newval == _value) return;
        _value = newval;
        _SuperT::border_style((_value ? border_styles::lowered : border_styles::raised));
        _SuperT::invalidate();
      }

    protected:

      explicit isa_toggle_button(window * pParent) : _SuperT(pParent){}
      
      void on_wm_create() override{
        _SuperT::border_style(border_styles::raised);
        _SuperT::on_wm_create();
      };

      void on_wm_mouse_up(const mouse_msg_param& m) override{
        if (mouse_msg_param::buttons::left != m.button) return _SuperT::on_wm_mouse_up(m);
        _SuperT::on_wm_mouse_up(m);
        value(!_value);
      };

    private:

      bool _value = false;
    };
  }

  namespace _{

    template <> struct policy_traits<policy::isa_toggle_button>{
      using requires = policy_list<policy::isa_button>;
    };

  }


  struct toggle_button : window_impl<toggle_button, policy::isa_toggle_button>{
    explicit toggle_button(window * pParent) : window_impl(pParent){}
  };

}
