#pragma once

namespace wtf{



  template <typename _ImplT, policy..._Policies>
  class window<_ImplT, policy::isa_button, _Policies...> :
    public window<_ImplT, policy::isa_label, _Policies...>{
    using __super_t = window<_ImplT, policy::isa_label, _Policies...>;
  public:
    explicit window(iwindow * hParent) : __super_t(hParent){
      border_style(border_styles::raised);
    }

  protected:
    
    virtual void on_wm_click(const mouse_msg_param& m) override{
      __super_t::on_wm_click(m);
    }

    virtual void on_wm_mouse_down(const mouse_msg_param& oParam) override{
      if (oParam.button == mouse_msg_param::buttons::left){
        border_style(border_styles::lowered);
        invalidate();
        ::SetCapture(*this);
      }
      __super_t::on_wm_mouse_down(oParam);
    }

    virtual void on_wm_mouse_up(const mouse_msg_param& oParam) override{
      if (oParam.button == mouse_msg_param::buttons::left){
        border_style(border_styles::raised);
        invalidate();
        ::ReleaseCapture();
      }
      __super_t::on_wm_mouse_up(oParam);
    }

  };

}
