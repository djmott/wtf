#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT>
    class wm_keyup : public _SuperT{
      
    public:

    protected:

      virtual void on_wm_keyup(UINT char_code, keyboard_msg_param, bool&) {}

      explicit wm_keyup(iwindow * pParent) : _SuperT(pParent){}

      void handle_msg(window_message& msg) override{
        if (WM_KEYUP == msg.umsg) on_wm_keyup(static_cast<UINT>(msg.wparam), *reinterpret_cast<keyboard_msg_param*>(&msg.lparam));
        _SuperT::handle_msg(msg);
      }

    };
  }
}