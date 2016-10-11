#pragma once

namespace wtf{

  struct push_button : wtf::window < push_button, policy::has_button_border, policy::has_size, policy::has_cursor, 
    policy::has_text, policy::has_font, policy::has_paint, policy::has_click>
  {

    explicit push_button(HWND hParent) : window(hParent, true){}
    
  };

}
