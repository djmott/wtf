#pragma once

namespace wtf{

  template <typename _ImplT, policy..._Policies>
  class window<_ImplT, policy::isa_label, _Policies...> : 
    public window<_ImplT, 
    policy::isa_panel,
    policy::has_text,
    policy::has_click,
    _Policies...>
  {
    using __super_t = window<_ImplT, policy::isa_panel, policy::has_text, policy::has_click, _Policies...>;
    template <typename, policy ... > friend class window;
  public:

  protected:
    explicit window(iwindow * pParent) : __super_t(pParent){}
  };

/*
struct label : window<label, iwindow, policy::has_size, policy::has_move, wm_create,
    policy::has_show, policy::has_text, policy::has_border, policy::has_click, policy::has_zorder,
    wm_mouse_move, policy::has_background, wm_erasebkgnd, wm_mouse_down,
    wm_mouse_up, wm_nccalcsize, wm_ncpaint, wm_paint, wm_erasebkgnd,
    wm_setcursor>{

    explicit label(iwindow * hParent) : window(hParent){}

  protected:
    virtual LRESULT on_wm_create(bool& bHandled) override{ return window::on_wm_create(bHandled); }
    virtual LRESULT on_wm_mouse_move(const mouse_msg_param& m, bool& b) override{ return window::on_wm_mouse_move(m, b); }
    virtual void on_wm_click(const mouse_msg_param& m) override{ return window::on_wm_click(m); }

  };*/

}
