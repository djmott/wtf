#pragma once

namespace wtf{

  template <typename _ImplT, policy..._Policies>
  class window<_ImplT, policy::wm_dblclick, _Policies...>
    : public window_impl<_ImplT, _Policies...>
  {
    using __super_t = window_impl<_ImplT, _Policies...>;
    template <typename, policy ... > friend class window_impl;

  protected:
    virtual void on_wm_dblclick(const mouse_msg_param&) {}

    explicit window(iwindow * pParent) : __super_t(pParent){}

    virtual void handle_msg(window_message& msg) override{
      if (WM_LBUTTONDBLCLK == msg.umsg){
        on_wm_dblclick(mouse_msg_param(msg.wparam, msg.lparam, mouse_msg_param::buttons::left));
      } else if (WM_MBUTTONDBLCLK == msg.umsg){
        on_wm_dblclick(mouse_msg_param(msg.wparam, msg.lparam, mouse_msg_param::buttons::middle));
      } else if (WM_RBUTTONDBLCLK == msg.umsg){
        on_wm_dblclick(mouse_msg_param(msg.wparam, msg.lparam, mouse_msg_param::buttons::right));
      }
    }
  };
}
