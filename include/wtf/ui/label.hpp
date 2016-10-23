#pragma once
namespace wtf{

    struct label : window<label, policy::has_size, policy::has_move, messages::wm_create,
      policy::has_show, policy::has_text, policy::has_border, policy::has_click, policy::has_zorder,
      messages::wm_mouse_move, policy::has_background, messages::wm_erasebkgnd, messages::wm_mouse_down, 
      messages::wm_mouse_up, messages::wm_nccalcsize, messages::wm_ncpaint, messages::wm_paint, messages::wm_erasebkgnd,
      messages::wm_setcursor>
    {

      explicit label(window<void,void> * hParent) : window(hParent){}

    protected:
      virtual LRESULT on_wm_create(bool& bHandled) override{ return window::on_wm_create(bHandled); }
      virtual LRESULT on_wm_mouse_move(const mouse_msg_param& m, bool& b) override{ return window::on_wm_mouse_move(m, b); }
      virtual void on_wm_click(const mouse_msg_param& m) override{ return window::on_wm_click(m); }

    };

}
