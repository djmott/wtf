#pragma once

namespace wtf{


    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::wm_mouse_up, _Policies...> 
      : public window_impl<_ImplT, _Policies...>
    {
      using __super_t = window_impl<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window_impl;
    public:

    protected:
      virtual void on_wm_mouse_up(const mouse_msg_param&) = 0{}

      explicit window(iwindow * pParent) : __super_t(pParent){}

      virtual void handle_msg(window_message& msg) override{
        if (WM_LBUTTONUP == msg.umsg) on_wm_mouse_up(mouse_msg_param(msg.wparam, msg.lparam, mouse_msg_param::buttons::left));
        else if (WM_MBUTTONUP == msg.umsg) on_wm_mouse_up(mouse_msg_param(msg.wparam, msg.lparam, mouse_msg_param::buttons::middle));
        else if (WM_RBUTTONUP == msg.umsg) on_wm_mouse_up(mouse_msg_param(msg.wparam, msg.lparam, mouse_msg_param::buttons::right));
      }
    };
  }
