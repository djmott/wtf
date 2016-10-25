#pragma once

namespace wtf{

     template <typename _ImplT, policy..._Policies> 
     class window<_ImplT, policy::wm_keyup, _Policies...> : public window<_ImplT, _Policies...>{
      using __super_t = window<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window;
     public:

    protected:

      virtual void on_wm_keyup(UINT char_code, keyboard_msg_param, bool&){}

      explicit window(iwindow * pParent) : __super_t(pParent){}

      LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam){
        if (WM_KEYUP==umsg) on_wm_keyup(static_cast<UINT>(wparam), *reinterpret_cast<keyboard_msg_param*>(&lparam));
        return __super_t::handle_message(hwnd, umsg, wparam, lparam);
      }

    };
  }
