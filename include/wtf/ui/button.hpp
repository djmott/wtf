#pragma once

namespace wtf{

  struct button : wtf::window < button, policy::has_button_border, policy::has_size, 
    policy::has_cursor, policy::has_text, policy::has_font, policy::has_paint, policy::has_click, 
    policy::has_mouse_move, policy::has_mouse_up, policy::has_mouse_down, policy::has_border, 
    policy::has_zorder, policy::has_create, policy::has_move>
  {

    explicit button(window<void> * hParent) : window(hParent){}
    
  };

}
