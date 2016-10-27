#pragma once

namespace wtf{

  template <typename _ImplT, typename _SuperT>
  class wm_char : public _SuperT
  {
    template <typename, policy ... > friend class window_impl;

  protected:

    virtual void on_wm_char(UINT char_code, keyboard_msg_param)=0{}

    wm_char(iwindow * pParent) : _SuperT(pParent){}

    virtual void handle_msg(window_message& msg) override{
      if (WM_CHAR == msg.umsg) on_wm_char(static_cast<UINT>(msg.wparam), *reinterpret_cast<keyboard_msg_param*>(&msg.lparam));
    }

  };
}
