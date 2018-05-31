/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{

    template <typename _SuperT>
    struct isa_button : _SuperT{

    protected:

      explicit isa_button(window * hParent) : _SuperT(hParent){
        _SuperT::border_style(border_styles::raised);
      }

      void on_wm_mouse_down(const mouse_msg_param<coord_frame::client>& oParam) override{
        if (oParam.button == mouse_buttons::left){
          _SuperT::border_style(border_styles::lowered);
          _SuperT::invalidate();
          ::SetCapture(*this);
        }
        _SuperT::on_wm_mouse_down(oParam);
      }

      void on_wm_mouse_up(const mouse_msg_param<coord_frame::client>& oParam) override{
        if (oParam.button == mouse_buttons::left){
          _SuperT::border_style(border_styles::raised);
          _SuperT::invalidate();
          ::ReleaseCapture();
        }
        _SuperT::on_wm_mouse_up(oParam);
      }

    };
  }

  namespace _{
    template <> struct policy_traits<policy::isa_button>{
      using requires = policy_list<policy::isa_label>;
    };
  }

  struct button : window_impl<button, policy::isa_button>{
    explicit button(window * pParent) noexcept : window_impl(pParent){}
  };

}
