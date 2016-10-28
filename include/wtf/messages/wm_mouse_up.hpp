#pragma once

namespace wtf{

  namespace policy{
    template <typename _ImplT, typename _SuperT>
    class wm_mouse_up : public _SuperT{

      
    public:

    protected:
      virtual void on_wm_mouse_up(const mouse_msg_param&) = 0{}

      explicit wm_mouse_up(iwindow * pParent) : _SuperT(pParent){}

      virtual void handle_msg(window_message& msg) override{
        if (WM_LBUTTONUP == msg.umsg) on_wm_mouse_up(mouse_msg_param(msg.wparam, msg.lparam, mouse_msg_param::buttons::left));
        else if (WM_MBUTTONUP == msg.umsg) on_wm_mouse_up(mouse_msg_param(msg.wparam, msg.lparam, mouse_msg_param::buttons::middle));
        else if (WM_RBUTTONUP == msg.umsg) on_wm_mouse_up(mouse_msg_param(msg.wparam, msg.lparam, mouse_msg_param::buttons::right));
      }
    };
  }
}