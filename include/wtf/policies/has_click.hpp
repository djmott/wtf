
/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once


namespace wtf{
  namespace policy{

    /** has_click
    * produces click events
    */
    template <typename _SuperT>
    struct has_click : _SuperT{

      callback<void(window *, mouse_msg_param<coord_frame::client>)> OnClick;

    protected:

      explicit has_click(window * pParent) : _SuperT(pParent){}

      virtual void on_wm_click(const mouse_msg_param<coord_frame::client>& p){ OnClick(this, p); }

      void on_wm_mouse_down(const mouse_msg_param<coord_frame::client>& oParam) override{
        _Down = oParam.button;
        _SuperT::on_wm_mouse_down(oParam);
      }

      void on_wm_mouse_up(const mouse_msg_param<coord_frame::client>& oParam) override{
        if (rect<coord_frame::client>::get(*this).is_in(oParam.position) && _Down == oParam.button){
          on_wm_click(oParam);
          _SuperT::on_wm_mouse_up(oParam);
          _Down = mouse_buttons::unspecified;
        }
      }

      mouse_buttons _Down = mouse_buttons::unspecified;
    };
  }

  namespace _{
    template <> struct policy_traits<policy::has_click>{
      using requires = policy_list<policy::wm_mouse_down, policy::wm_mouse_up>;
    };
  }
}