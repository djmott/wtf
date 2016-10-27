#pragma once


namespace wtf{


    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::wm_mouse_down, _Policies...> 
      : public window_impl<_ImplT, _Policies...>
    {
      using __super_t = window_impl<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window_impl;
    public:


    protected:

      virtual void on_wm_mouse_down(const mouse_msg_param&) = 0{}

      explicit window(iwindow * pParent) : __super_t(pParent){}

      virtual void handle_msg(window_message& msg) override{
        if (WM_LBUTTONDOWN == msg.umsg) on_wm_mouse_down(mouse_msg_param(msg.wparam, msg.lparam, mouse_msg_param::buttons::left));
        else if (WM_MBUTTONDOWN == msg.umsg) on_wm_mouse_down(mouse_msg_param(msg.wparam, msg.lparam, mouse_msg_param::buttons::middle));
        else if (WM_RBUTTONDOWN == msg.umsg) on_wm_mouse_down(mouse_msg_param(msg.wparam, msg.lparam, mouse_msg_param::buttons::right));
      }
    };
  }
