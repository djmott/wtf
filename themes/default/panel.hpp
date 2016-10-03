#pragma once

namespace wtf{
  namespace default_theme{
    struct panel : wtf::window<panel, has_size, has_border, has_background, has_paint, has_show>{
      panel(HWND parent) : window(parent){}
    };
  }
}