#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT>
    class wm_mouse_move : public _SuperT{

      
    public:

    protected:
      virtual void on_wm_mouse_move(const mouse_msg_param&){}

      explicit wm_mouse_move(iwindow * pParent) : _SuperT(pParent){}

      void handle_msg(window_message& msg) override{
        if (WM_MOUSEMOVE == msg.umsg){
          on_wm_mouse_move(mouse_msg_param(msg.wparam, msg.lparam, mouse_msg_param::buttons::unspecified));
        }
        _SuperT::handle_msg(msg);
      }
    };
  }
}