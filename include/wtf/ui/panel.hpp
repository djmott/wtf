#pragma once

namespace wtf{

  template <typename _ImplT, policy..._Policies>
  class window<_ImplT, policy::isa_panel, _Policies...> : 
    public window_impl<_ImplT,
    _Policies...,
    policy::wm_mouse_move,
    policy::wm_create,
    policy::wm_size,
    policy::wm_paint,
    policy::wm_erasebkgnd,
    policy::has_border,
    policy::has_move, 
    policy::has_invalidate,
    policy::has_size, 
    policy::has_show,
    policy::has_zorder,
    policy::has_click
    >
  {
    using __super_t = window_impl<_ImplT, _Policies..., policy::wm_mouse_move, policy::wm_create,
      policy::wm_size, policy::wm_paint, policy::wm_erasebkgnd, policy::has_border, policy::has_move,
       policy::has_invalidate, policy::has_size, policy::has_show,
      policy::has_zorder, policy::has_click>;
    template <typename, policy ... > friend class window_impl;

  protected:
    virtual void handle_msg(window_message& msg) override{}
    explicit window(iwindow * pParent) : __super_t(pParent){}


  };

  struct panel : window<panel, policy::isa_panel>{
    explicit panel(iwindow * pParent) : window(pParent){}
  };

}
