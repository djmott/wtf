#pragma once

namespace wtf{
  namespace policy{
    template <typename _SuperT, typename _ImplT>
    class wm_create : public _SuperT{
      
    public:

    protected:

      virtual void on_wm_create() {}

      explicit wm_create(iwindow * pParent) : _SuperT(pParent){}

      virtual void handle_msg(window_message& msg) override{
        if (WM_CREATE == msg.umsg) on_wm_create();
        _SuperT::handle_msg(msg);
      }

    };

  }
}