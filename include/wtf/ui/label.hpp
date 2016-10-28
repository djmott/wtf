#pragma once

namespace wtf{

  namespace policy{
    template <typename _ImplT, typename _SuperT>
    class isa_label : public _SuperT{

    protected:
      virtual void handle_msg(window_message& msg) override{}
      explicit isa_label(iwindow * pParent) : _SuperT(pParent){}
    };
  }

  struct label : window_impl<label, policy::isa_label, policy::has_click, policy::wm_mouse_down, policy::wm_mouse_up >{
    explicit label(iwindow * pParent) : window_impl(pParent){}
  };

}
