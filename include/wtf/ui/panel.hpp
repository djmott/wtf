#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT, typename _ImplT>
    class isa_panel : public _SuperT{

    protected:

      explicit isa_panel(iwindow * pParent) : _SuperT(pParent){}

    };
  }

  template <> struct policy_traits<policy::isa_panel>{
    using requires = policy_list<policy::has_border, policy::has_click, policy::wm_paint, policy::has_show,
    policy::has_size, policy::has_move, policy::wm_size, policy::has_invalidate, policy::wm_erasebkgnd,
    policy::has_zorder>;
  };

  struct panel : window_impl<panel, policy::isa_panel>
  {
    explicit panel(iwindow * pParent) : window_impl(pParent){}
  };

}
