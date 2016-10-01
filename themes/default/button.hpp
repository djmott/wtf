#pragma once

namespace wtf{
  namespace default_theme{
    struct button : wtf::window<button, has_move, has_background, has_tracking_border, has_click_event, has_text, has_paint_event>{


      button(HWND hParent) : window(hParent, true){}

      
    private:

    };

  }
}