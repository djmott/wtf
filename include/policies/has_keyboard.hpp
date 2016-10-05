#pragma once

namespace wtf{
  namespace policy{
    template <typename _SuperT> 
    struct has_keyboard : _SuperT{

      has_keyboard(const has_keyboard&) = delete;
      has_keyboard(has_keyboard&&) = delete;
      has_keyboard &operator=(const has_keyboard &) = delete;
      has_keyboard &operator=(has_keyboard&&) = delete;
      virtual ~has_keyboard() = default;
      has_keyboard() : _SuperT(){

      }

      enum class hotkey_modifiers{
        alt = MOD_ALT,
        control = MOD_CONTROL,
        shift = MOD_SHIFT,
        win = MOD_WIN,
      };

      struct key_data{
        uint32_t repeat_count : 16;
        uint32_t scan_code : 8;
        uint32_t extended_key : 1;
        uint32_t reserved : 4;
        uint32_t current_context : 1;
        uint32_t prev_context : 1;
        uint32_t transition_context : 1;
      };

      void set_focus() const{
        ::SetFocus(*this);
      }

    protected:

      virtual void CharEvent(TCHAR, key_data){}
      virtual void KeyDownEvent(TCHAR, key_data){}
      virtual void KeyUpEvent(TCHAR, key_data){}
      virtual void GotKeyboardFocusEvent(){}
      virtual void LoosingKeyboardFocusEvent(){}

      virtual LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM lparam, bool&){
        switch (umsg){
          case WM_CHAR:{
            CharEvent(static_cast<TCHAR>(wparam), *reinterpret_cast<key_data*>(&lparam)); break;
          }
          case WM_KEYDOWN:{
            KeyDownEvent(static_cast<TCHAR>(wparam), *reinterpret_cast<key_data*>(&lparam)); break;
          }
          case WM_KEYUP:{
            KeyUpEvent(static_cast<TCHAR>(wparam), *reinterpret_cast<key_data*>(&lparam)); break;
          }
          case WM_SETFOCUS:{
            GotKeyboardFocusEvent(); break;

          case WM_KILLFOCUS:{
            LoosingKeyboardFocusEvent(); break;
          }
          }
        }
        return 0; 
      }


    };
  }
}