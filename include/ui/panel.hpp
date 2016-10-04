#pragma once

namespace wtf{


    struct panel : wtf::window<panel,
      policy::has_size, policy::has_border, policy::has_paint, policy::has_show>
    {
      panel(HWND parent) : window(parent){}

    };


}
