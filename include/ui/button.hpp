#pragma once

namespace wtf{

  struct button : wtf::window < button, policy::has_button_border, policy::has_size, policy::has_cursor, 
    policy::has_text, policy::has_font, policy::has_paint, policy::has_click>
  {

    explicit button(iwindow * hParent) : window(hParent){}
    
  };

}
