#pragma once

namespace wtf{

    struct push_button : wtf::window<push_button, policy::has_size, policy::has_border,
      policy::has_click, policy::has_text, policy::has_paint, policy::has_mouse>
    {
      push_button(HWND hParent) : window(hParent, true){}

      
    private:
    };

}
