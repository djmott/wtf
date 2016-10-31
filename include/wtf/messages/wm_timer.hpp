#pragma once

namespace wtf{
  namespace policy{
    template <typename _SuperT>
    class wm_timer : public _SuperT{

      
    public:


    protected:
      explicit wm_timer(iwindow * pParent) : _SuperT(pParent){}

      virtual void on_wm_timer(UINT_PTR){}

      void handle_msg(window_message& msg) override{
        if (WM_TIMER == msg.umsg) on_wm_timer(static_cast<UINT_PTR>(msg.wparam));
        _SuperT::handle_msg(msg);
      }

    };

  }
}