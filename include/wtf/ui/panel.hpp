#pragma once

namespace wtf{

  namespace policy{
    template <typename _ImplT, typename _SuperT>
    class isa_panel : public _SuperT{

      

    protected:

      explicit isa_panel(iwindow * pParent) : _SuperT(pParent){}


    };
  }

  struct panel : window_impl<panel, policy::isa_panel, policy::wm_mouse_move, policy::wm_create,
    policy::wm_size, policy::wm_paint, policy::wm_erasebkgnd, policy::has_border, policy::has_move,
    policy::has_invalidate, policy::has_size, policy::has_show, policy::has_zorder, policy::has_click,
    policy::wm_mouse_down, policy::wm_mouse_up, policy::wm_mouse_leave, policy::wm_nccalcsize, 
    policy::wm_ncpaint
  >
  {
    explicit panel(iwindow * pParent) : window_impl(pParent){}
  };

}
