#pragma once
namespace wtf{
  namespace policy{

    template <typename _SuperT>
    struct isa_combobox{};

  }

  namespace _{
    template <> struct policy_traits<policy::isa_combobox>{
      using requires = policy_list<>;
    };
  }

  struct combobox : window_impl<combobox, policy::isa_combobox>{
  private:
    using __super_t = window_impl<combobox, policy::isa_combobox>;
  public:
    combobox(window * parent) : __super_t(parent){}
  };

}