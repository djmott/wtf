#pragma once
namespace wtf{
  namespace policy{
    struct mouse_event{

      enum class buttons{
        unspecified = 0,
        left = MK_LBUTTON,
        right = MK_RBUTTON,
        middle = MK_MBUTTON,
      };

      enum class key_states{
        unspecified = 0,
        control = MK_CONTROL,
        left = MK_LBUTTON,
        middle = MK_MBUTTON,
        right = MK_RBUTTON,
        shift = MK_SHIFT,
        xbutton1 = MK_XBUTTON1,
        xbutton2 = MK_XBUTTON2,
      };

      buttons button; //the button that caused the event
      weak_enum<key_states> key_state; //state of all buttons

      point::client_coords position;

      mouse_event(WPARAM wparam, LPARAM lparam, buttons oButton = buttons::unspecified)
        : button(oButton), key_state(static_cast<mouse_event::key_states>(LOWORD(wparam))),
        position(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam)){}
      
      mouse_event(const mouse_event& src)
        : button(src.button), key_state(src.key_state), position(src.position){}

      mouse_event& operator=(const mouse_event& src){
        button = src.button;
        key_state = src.key_state;
        position = src.position;
        return *this;
      }
    };
  }
}