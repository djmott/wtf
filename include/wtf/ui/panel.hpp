#pragma once

namespace wtf{


    struct panel : wtf::window<panel,
      policy::has_size, policy::has_border, policy::has_show, policy::has_zorder, policy::has_click, 
      policy::has_move, messages::wm_create, messages::wm_paint, messages::wm_size, policy::has_background,
      messages::wm_nccalcsize, messages::wm_ncpaint, messages::wm_erasebkgnd, messages::wm_mouse_down,
      messages::wm_mouse_up>
    {
      explicit panel(window<void,void> * pParent) : window(pParent){}
    protected:
      virtual void on_wm_click(const mouse_msg_param& m) override{ window::on_wm_click(m); }
      virtual LRESULT on_wm_create(bool& bHandled) override{ return window::on_wm_create(bHandled); }
      virtual LRESULT on_wm_paint(const device_context& dc, const paint_struct& ps, bool& bHandled) override{
        return window::on_wm_paint(dc, ps, bHandled);
      }
      virtual LRESULT on_wm_size(const point<coord_frame::client>& p, bool& bHandled)override{
        return window::on_wm_size(p, bHandled);
      }

    };


}
