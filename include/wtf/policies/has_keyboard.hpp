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

    protected:
      virtual void OnCharPress(TCHAR, key_data){}
      virtual void OnKeyDown(TCHAR, key_data){}
      virtual void OnKeyUp(TCHAR, key_data){}
      virtual void OnGotKeyboardFocus(){}
      virtual void OnLostKeyboardFocus(){}


      has_keyboard(window<void> * pParent) : _SuperT(pParent){}


      LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM lparam, bool&){
        switch (umsg){
          case WM_CHAR:{
            OnCharPress(static_cast<TCHAR>(wparam), *reinterpret_cast<key_data*>(&lparam)); break;
          }
          case WM_KEYDOWN:{
            OnKeyDown(static_cast<TCHAR>(wparam), *reinterpret_cast<key_data*>(&lparam)); break;
          }
          case WM_KEYUP:{
            OnKeyUp(static_cast<TCHAR>(wparam), *reinterpret_cast<key_data*>(&lparam)); break;
          }
          case WM_SETFOCUS:{
            OnGotKeyboardFocus(); break;
          }
          case WM_KILLFOCUS:{
            OnLostKeyboardFocus(); break;
          }
        }
        return 0; 
      }


    };
  }
}