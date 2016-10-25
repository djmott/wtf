#pragma once

namespace wtf{

  template <typename _ImplT, policy..._Policies>
  class window<_ImplT, policy::isa_panel, _Policies...> : 
    public window<_ImplT, 
    policy::has_border, 
    policy::has_move, 
    policy::has_background, 
    policy::has_invalidate,
    policy::wm_create,
    policy::has_size, 
    policy::has_show,
    policy::wm_size,
    _Policies...>
  {
    using __super_t = window<_ImplT, policy::has_border, policy::has_move, policy::has_background, 
      policy::has_invalidate, policy::wm_create, policy::has_size, policy::has_show, 
      policy::wm_size, _Policies...>;
    template <typename, policy ... > friend class window;
  public:


  protected:
    explicit window(iwindow * pParent) : __super_t(pParent){}



  };


}
