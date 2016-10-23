#pragma once

namespace wtf{


    struct panel : wtf::window<panel,
      policy::has_size, policy::has_border, policy::has_paint, policy::has_show, policy::has_zorder, 
      policy::has_click, policy::has_create, policy::has_move>
    {
      explicit panel(window<void> * pParent) : window(pParent){}
    protected:
      virtual void on_wm_click(const mouse_msg_param& m) override{
        window::on_wm_click(m);
      }

    };


}
