#pragma once

namespace wtf{
  namespace policy{
    template <typename _ImplT, typename _SuperT>
    class wm_create : public _SuperT{
      
    public:

    protected:

      virtual void on_wm_create() = 0{}

      explicit wm_create(iwindow * pParent) : _SuperT(pParent){}

      virtual void handle_msg(window_message& msg) override{
        if (WM_CREATE == msg.umsg) on_wm_create();
      }

    };

  }
}