#pragma once

namespace wtf{
  namespace default_theme{

    struct panel : wtf::window<panel, has_size, has_border>{


      panel(HWND parent) : window(parent){}

    protected:
    };

  }
}