#pragma once

namespace wtf{

  namespace policy{

    template <typename _ImplT, typename _SuperT>
    class isa_button : public _SuperT{

    public:

      explicit isa_button(iwindow * hParent) : _SuperT(hParent){
        border_style(border_styles::raised);
      }

    protected:
      virtual void handle_msg(window_message& msg) override{}

      virtual void on_wm_click(const mouse_msg_param& m) override{
        _SuperT::on_wm_click(m);
      }

      virtual void on_wm_mouse_down(const mouse_msg_param& oParam) override{
        if (oParam.button == mouse_msg_param::buttons::left){
          border_style(border_styles::lowered);
          invalidate();
          ::SetCapture(*this);
        }
        _SuperT::on_wm_mouse_down(oParam);
      }

      virtual void on_wm_mouse_up(const mouse_msg_param& oParam) override{
        if (oParam.button == mouse_msg_param::buttons::left){
          border_style(border_styles::raised);
          invalidate();
          ::ReleaseCapture();
        }
        _SuperT::on_wm_mouse_up(oParam);
      }

    };
  }

  struct button 
    : window_impl<button, policy::isa_button, policy::has_click, policy::wm_mouse_down, policy::wm_mouse_up,
    policy::wm_paint>
  {
    explicit button(iwindow * pParent) : window_impl(pParent){}
  };

}
