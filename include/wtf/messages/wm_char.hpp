#pragma once

namespace wtf{
  namespace policy{
    template <typename _SuperT>
    class wm_char : public _SuperT{

    protected:

      virtual void on_wm_char(UINT char_code, keyboard_msg_param) {}

      wm_char(iwindow * pParent) : _SuperT(pParent){}

      void handle_msg(window_message& msg) override{
        if (WM_CHAR == msg.umsg) on_wm_char(static_cast<UINT>(msg.wparam), *reinterpret_cast<keyboard_msg_param*>(&msg.lparam));
        _SuperT::handle_msg(msg);
      }

    };
  }
}