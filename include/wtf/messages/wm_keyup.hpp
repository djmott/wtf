#pragma once

namespace wtf{

     template <typename _ImplT, policy..._Policies> 
     class window<_ImplT, policy::wm_keyup, _Policies...> 
       : public window_impl<_ImplT, _Policies...>
     {
       using __super_t = window_impl<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window_impl;
     public:

    protected:

      virtual void on_wm_keyup(UINT char_code, keyboard_msg_param, bool&) = 0{}

      explicit window(iwindow * pParent) : __super_t(pParent){}

      virtual void handle_msg(window_message& msg) override{
        if (WM_KEYUP == msg.umsg) on_wm_keyup(static_cast<UINT>(msg.wparam), *reinterpret_cast<keyboard_msg_param*>(&msg.lparam));
      }

    };
  }
