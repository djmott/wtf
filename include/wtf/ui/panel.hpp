#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT>
    class isa_panel : public _SuperT{

    protected:

      explicit isa_panel(iwindow * pParent) : _SuperT(pParent){}

    };
  }

  template <> struct policy_traits<policy::isa_panel>{
    using requires = policy_list<policy::has_border, policy::has_click,  policy::has_show,
    policy::has_size, policy::has_move,  policy::has_invalidate, policy::has_background,
    policy::has_zorder, policy::wm_size, policy::wm_paint, policy::wm_create>;
  };


}
