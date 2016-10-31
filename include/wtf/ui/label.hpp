#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT, typename _ImplT>
    class isa_label : public _SuperT{

    protected:
      explicit isa_label(iwindow * pParent) : _SuperT(pParent){}
    };
  }

  template <> struct policy_traits<policy::isa_label>{
    using requires = policy_list<policy::has_text, policy::isa_panel, policy::wm_create>;
  };

  struct label : window_impl<label, policy::isa_label >{
    explicit label(iwindow * pParent) : window_impl(pParent){}
  };

}
