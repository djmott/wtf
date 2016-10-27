#pragma once


namespace wtf{
  template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::isa_toggle_button, _Policies...> 
      : public window_impl<_ImplT, _Policies..., policy::isa_button>

    {

      using __super_t = window_impl<_ImplT, _Policies..., policy::isa_button>;
    public:

      explicit window(iwindow * pParent) : __super_t(pParent) {}
    
      bool value() const{ return _value; }
      void value(bool newval){
        if (newval == _value) return;
        _value = newval;
        border_style((_value ? border_styles::lowered : border_styles::raised));
        invalidate();
      }

    protected:

      virtual void handle_msg(window_message& msg) override{}
      virtual void on_wm_create() override{
        border_style(border_styles::raised); 
        __super_t::on_wm_create();
      };

      virtual void on_wm_mouse_up(const mouse_msg_param& m) override{
        if (mouse_msg_param::buttons::left != m.button) return __super_t::on_wm_mouse_up(m);
        value(!_value);
        __super_t::on_wm_mouse_up(m);
      };

    private:

      bool _value = false;
    };

    struct toggle_button : window<toggle_button, policy::isa_toggle_button>{
      explicit toggle_button(iwindow * pParent) : window(pParent){}
    };

}
