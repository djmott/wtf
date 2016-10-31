#pragma once


namespace wtf{
  namespace policy{
    template <typename _SuperT>
    class isa_toggle_button : public _SuperT{

    public:

      explicit isa_toggle_button(iwindow * pParent) : _SuperT(pParent){}

      bool value() const{ return _value; }
      void value(bool newval){
        if (newval == _value) return;
        _value = newval;
	      _SuperT::border_style((_value ? border_styles::lowered : border_styles::raised));
	      _SuperT::invalidate();
      }

    protected:

      void on_wm_create() override{
	      _SuperT::border_style(border_styles::raised);
        _SuperT::on_wm_create();
      };

      void on_wm_mouse_up(const mouse_msg_param& m) override{
        if (mouse_msg_param::buttons::left != m.button) return _SuperT::on_wm_mouse_up(m);
        value(!_value);
        _SuperT::on_wm_mouse_up(m);
      };

    private:

      bool _value = false;
    };
  }


  template <> struct policy_traits<policy::isa_toggle_button>{
    using requires = policy_list<policy::isa_button>;
  };

  struct toggle_button : window_impl<toggle_button, policy::isa_toggle_button>{
    explicit toggle_button(iwindow * pParent) : window_impl(pParent){}
  };

}
