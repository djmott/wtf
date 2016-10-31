#pragma once

namespace wtf{
  namespace policy{
    template <typename _SuperT, typename _ImplT>
    class wm_setfocus : public _SuperT{

      
    public:

    protected:

      virtual void on_wm_setfocus(HWND) {}

      explicit wm_setfocus(iwindow * pParent) : _SuperT(pParent){}

      virtual void handle_msg(window_message& msg) override{
        if (WM_SETFOCUS == msg.umsg){
          on_wm_setfocus(reinterpret_cast<HWND>(msg.wparam));
        }
        _SuperT::handle_msg(msg);
      }
    };
  }
}
