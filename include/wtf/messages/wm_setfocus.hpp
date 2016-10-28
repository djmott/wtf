#pragma once

namespace wtf{
  namespace policy{
    template <typename _ImplT, typename _SuperT>
    class wm_setfocus : public _SuperT{

      
    public:

    protected:

      virtual void on_wm_setfocus(HWND) = 0{}

      explicit wm_setfocus(iwindow * pParent) : _SuperT(pParent){}

      virtual void handle_msg(window_message& msg) override{
        if (WM_SETFOCUS == msg.umsg){
          on_wm_setfocus(reinterpret_cast<HWND>(msg.wparam));
        }
      }
    };
  }
}
