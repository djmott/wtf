#pragma once

namespace wtf{
  namespace policy{
    template <typename _SuperT>
    class wm_close : public _SuperT{

      void handle_msg(window_message& msg) override{
        if (WM_CLOSE == msg.umsg) on_wm_close();
        _SuperT::handle_msg(msg);
      }

    protected:

      virtual void on_wm_close() {}

      explicit wm_close(iwindow * pParent) : _SuperT(pParent){}


    };

  }
}