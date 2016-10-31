#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT, typename _ImplT>
    class wm_destroy : public _SuperT{
      

    protected:

      virtual void on_wm_destroy(){}

      explicit wm_destroy(iwindow * pParent) : _SuperT(pParent){}

      virtual void handle_msg(window_message& msg) override{
        if (WM_DESTROY == msg.umsg){
          on_wm_destroy();
        }
        _SuperT::handle_msg(msg);
      }

    };

  }
}