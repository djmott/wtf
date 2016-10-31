#pragma once


namespace wtf{
  /** has_click
  * produces click events
  */
  namespace policy{
    template <typename _SuperT, typename _ImplT>
    class has_click : public _SuperT{

      
    public:


    protected:

      explicit has_click(iwindow * pParent) : _SuperT(pParent){}

      virtual void on_wm_click(const mouse_msg_param&){}

      virtual void on_wm_mouse_down(const mouse_msg_param& oParam) override{
        _Down = oParam.button;
        _SuperT::on_wm_mouse_down(oParam);
      }

      virtual void on_wm_mouse_up(const mouse_msg_param& oParam) override{
        if (rect<coord_frame::client>::get(*this).is_in(oParam.position) && _Down == oParam.button){
          on_wm_click(oParam);
          _SuperT::on_wm_mouse_up(oParam);
          _Down = mouse_msg_param::buttons::unspecified;
        }
      }

      mouse_msg_param::buttons _Down = mouse_msg_param::buttons::unspecified;
    };
  }

  template <> struct policy_traits<policy::has_click>{
    using requires = policy_list<policy::wm_mouse_down, policy::wm_mouse_up>;
  };
}