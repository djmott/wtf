#pragma once

namespace wtf{

  template <typename _ImplT, policy..._Policies>
  class window<_ImplT, policy::isa_label, _Policies...> 
    : public window_impl<_ImplT, _Policies..., policy::isa_panel,policy::has_text>
  {
    using __super_t = window_impl<_ImplT, _Policies..., policy::isa_panel, policy::has_text>;
    template <typename, policy ... > friend class window;
    template <typename, policy ... > friend class window_impl;

  protected:
    virtual void handle_msg(window_message& msg) override{}
    explicit window(iwindow * pParent) : __super_t(pParent){}
  };

  struct label : window<label, policy::isa_label>{
    explicit label(iwindow * pParent) : window(pParent){}
  };

}
