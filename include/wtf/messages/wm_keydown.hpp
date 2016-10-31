#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT>
    class wm_keydown : public _SuperT{
      
    public:

    protected:

      virtual void on_wm_keydown(UINT char_code, keyboard_msg_param) {}

      explicit wm_keydown(iwindow * pParent) : _SuperT(pParent){}

      void handle_msg(window_message& msg) override{
        if (WM_KEYDOWN == msg.umsg) on_wm_keydown(static_cast<UINT>(msg.wparam), *reinterpret_cast<keyboard_msg_param*>(&msg.lparam));
        _SuperT::handle_msg(msg);
      }

    };
  }
}