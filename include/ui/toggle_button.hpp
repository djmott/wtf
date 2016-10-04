#pragma once

namespace wtf{

    struct toggle_button : wtf::window<toggle_button, policy::has_size, policy::has_border,
      policy::has_click, policy::has_text, policy::has_paint, policy::has_mouse>
    {
      toggle_button(HWND hParent) : window(hParent, true), _button_state(button_states::up){}

      enum class button_states{
        up,
        down,
      };
      button_states button_state() const{ return _button_state; }
      void button_state(button_states newval) { _button_state=newval; }
    private:
      button_states _button_state;
    };

}
