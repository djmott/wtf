#pragma once

namespace wtf{

  namespace policy{

    template <typename _SuperT>
    class isa_button : public _SuperT{

    public:

      explicit isa_button(iwindow * hParent) : _SuperT(hParent){
	      _SuperT::border_style(border_styles::raised);
      }

    protected:

      void on_wm_mouse_down(const mouse_msg_param& oParam) override{
        if (oParam.button == mouse_msg_param::buttons::left){
	        _SuperT::border_style(border_styles::lowered);
	        _SuperT::invalidate();
          ::SetCapture(*this);
        }
        _SuperT::on_wm_mouse_down(oParam);
      }

      void on_wm_mouse_up(const mouse_msg_param& oParam) override{
        if (oParam.button == mouse_msg_param::buttons::left){
	        _SuperT::border_style(border_styles::raised);
	        _SuperT::invalidate();
          ::ReleaseCapture();
        }
        _SuperT::on_wm_mouse_up(oParam);
      }

    };
  }

  template <> struct policy_traits<policy::isa_button>{
    using requires = policy_list<policy::isa_label>;
  };

}
