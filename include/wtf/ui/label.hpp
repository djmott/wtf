#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT>
    class isa_label : public _SuperT{

    protected:
      explicit isa_label(iwindow * pParent) : _SuperT(pParent){}
    };
  }

  template <> struct policy_traits<policy::isa_label>{
    using requires = policy_list<policy::isa_panel, policy::has_text>;
  };


}
