#pragma once


namespace wtf{
  /** has_click
  * produces click events
  */
  template <typename _ImplT, policy..._Policies>
  class window<_ImplT, policy::has_click, _Policies...> 
    : public window_impl<_ImplT, _Policies..., policy::wm_mouse_down, policy::wm_mouse_up>
  {
    using __super_t = window_impl<_ImplT, _Policies..., policy::wm_mouse_down, policy::wm_mouse_up>;
    template <typename, policy ... > friend class window_impl;
  public:


  protected:

    explicit window(iwindow * pParent) : __super_t(pParent){}

    virtual void on_wm_click(const mouse_msg_param&){}

    virtual void on_wm_mouse_down(const mouse_msg_param& oParam) override{
      _Down = oParam.button;
      __super_t::on_wm_mouse_down(oParam);
    }

    virtual void on_wm_mouse_up(const mouse_msg_param& oParam) override{
      if (rect<coord_frame::client>::get(*this).is_in(oParam.position) && _Down == oParam.button){
        on_wm_click(oParam);
        __super_t::on_wm_mouse_up(oParam);
        _Down = mouse_msg_param::buttons::unspecified;
      }
    }

    mouse_msg_param::buttons _Down = mouse_msg_param::buttons::unspecified;
  };
}
