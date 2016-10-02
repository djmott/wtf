#pragma once

namespace wtf{
  namespace default_theme{
    struct text_box : wtf::window<text_box, has_size, has_background, has_tracking_border, has_click_event, has_text, has_paint>{


      text_box(HWND hParent) : window(hParent, true){}


    private:

    };

  }
}