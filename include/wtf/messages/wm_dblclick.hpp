#pragma once

namespace wtf{
  namespace policy{
    template <typename _ImplT, typename _SuperT>
    class wm_dblclick : public _SuperT{

      

    protected:
      virtual void on_wm_dblclick(const mouse_msg_param&) = 0{}

      explicit wm_dblclick(iwindow * pParent) : _SuperT(pParent){}

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
}
