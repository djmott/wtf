#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT, typename _ImplT>
    class wm_mouse_wheel : public _SuperT{

    protected:

      virtual void on_wm_mouse_wheel(int16_t /*delta*/, const mouse_msg_param&) {}

      explicit wm_mouse_wheel(iwindow * pParent) : _SuperT(pParent){}

      virtual void handle_msg(window_message& msg) override{
        if (WM_MOUSEWHEEL == msg.umsg) on_wm_mouse_wheel(static_cast<int16_t>(HIWORD(msg.wparam)), mouse_msg_param(msg.wparam, msg.lparam, mouse_msg_param::buttons::unspecified));
        _SuperT::handle_msg(msg);
      }
    };
  }
}