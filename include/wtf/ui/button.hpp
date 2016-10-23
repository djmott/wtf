#pragma once

namespace wtf{

  namespace ui{
    struct button : wtf::window < button, policy::has_button_border, policy::has_size,
      policy::has_text, policy::has_font, policy::has_click,
      messages::wm_mouse_move, messages::wm_mouse_up, messages::wm_mouse_down, policy::has_border,
      policy::has_zorder, messages::wm_create, policy::has_move, policy::has_background, messages::wm_erasebkgnd,
      messages::wm_ncpaint, messages::wm_nccalcsize, messages::wm_paint, messages::wm_mouse_leave >
    {

      explicit button(window<void,void> * hParent) : window(hParent){}

    }; 

  }
}