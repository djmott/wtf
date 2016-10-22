#pragma once

#define WTF_SIMPLE_WM_HANDLER( _name, _wm, _member) \
  template <typename _SuperT, typename> struct _name : _SuperT{ \
  protected: \
    virtual LRESULT _member(bool&) = 0; \
    LRESULT handle_message(HWND, UINT umsg, WPARAM, LPARAM, bool & bhandled) { \
      if (_wm == umsg) return _member(bhandled); \
      return 0; \
    } \
  };


namespace wtf{
  namespace messages{

    struct mouse_msg_param{

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

      point<coord_frame::client> position;

      mouse_msg_param(WPARAM wparam, LPARAM lparam, buttons oButton = buttons::unspecified)
        : button(oButton), key_state(static_cast<mouse_msg_param::key_states>(LOWORD(wparam))),
        position(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam)){}

      mouse_msg_param(const mouse_msg_param& src)
        : button(src.button), key_state(src.key_state), position(src.position){}

      mouse_msg_param& operator=(const mouse_msg_param& src){
        button = src.button;
        key_state = src.key_state;
        position = src.position;
        return *this;
      }
    };


    struct keyboard_msg_param{
      uint32_t repeat_count : 16;
      uint32_t scan_code : 8;
      uint32_t extended_key : 1;
      uint32_t reserved : 4;
      uint32_t current_context : 1;
      uint32_t prev_context : 1;
      uint32_t transition_context : 1;
    };


    WTF_SIMPLE_WM_HANDLER(wm_close, WM_CLOSE, on_close);
    WTF_SIMPLE_WM_HANDLER(wm_create, WM_CREATE, on_create);
  }
}