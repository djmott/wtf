#pragma once

namespace wtf{


    struct panel : wtf::window<panel,
      policy::has_size, policy::has_border, policy::has_paint, policy::has_show, policy::has_zorder, 
      policy::has_click, policy::has_create>
    {
      explicit panel(iwindow * pParent) : window(pParent){}

    };


}
