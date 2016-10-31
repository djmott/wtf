#pragma once


namespace wtf{

  namespace policy{
    template <typename _SuperT>
    class wm_mouse_down : public _SuperT{
    protected:

      virtual void on_wm_mouse_down(const mouse_msg_param&) {}

      explicit wm_mouse_down(iwindow * pParent) : _SuperT(pParent){}

      void handle_msg(window_message& msg) override{
        if (WM_LBUTTONDOWN == msg.umsg) on_wm_mouse_down(mouse_msg_param(msg.wparam, msg.lparam, mouse_msg_param::buttons::left));
        else if (WM_MBUTTONDOWN == msg.umsg) on_wm_mouse_down(mouse_msg_param(msg.wparam, msg.lparam, mouse_msg_param::buttons::middle));
        else if (WM_RBUTTONDOWN == msg.umsg) on_wm_mouse_down(mouse_msg_param(msg.wparam, msg.lparam, mouse_msg_param::buttons::right));
        _SuperT::handle_msg(msg);
      }
    };
  }
}