#pragma once

namespace wtf{
  namespace policy{
    template <typename _SuperT, typename _ImplT>
    struct has_keyboard : _SuperT{

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

      callback<void(TCHAR, key_data)> OnCharPress;
      callback<void(TCHAR, key_data)> OnKeyDown;
      callback<void(TCHAR, key_data)> OnKeyUp;
      callback<void()> OnGotKeyboardFocus;
      callback<void()> OnLostKeyboardFocus;

    protected:
      has_keyboard(iwindow * pParent) : _SuperT(pParent){}

      virtual void CharEvent(TCHAR c, key_data d){ OnCharPress(c, d); }
      virtual void KeyDownEvent(TCHAR c, key_data d){ OnKeyDown(c, d); }
      virtual void KeyUpEvent(TCHAR c, key_data d){ OnKeyUp(c, d); }
      virtual void GotKeyboardFocusEvent(){ OnGotKeyboardFocus(); }
      virtual void LoosingKeyboardFocusEvent(){ OnLostKeyboardFocus(); }

      LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM lparam, bool&){
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
          }
          case WM_KILLFOCUS:{
            LoosingKeyboardFocusEvent(); break;
          }
        }
        return 0; 
      }


    };
  }
}