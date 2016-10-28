#pragma once

namespace wtf{
  namespace policy{
    template <typename _ImplT, typename _SuperT>
    class wm_close : public _SuperT{

      virtual void handle_msg(window_message& msg) override{
        if (WM_CLOSE == msg.umsg) on_wm_close();
      }

    protected:

      virtual void on_wm_close() = 0{}

      explicit wm_close(iwindow * pParent) : _SuperT(pParent){}


    };

  }
}