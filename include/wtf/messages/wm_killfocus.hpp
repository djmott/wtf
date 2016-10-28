#pragma once

namespace wtf{

  namespace policy{
    template <typename _ImplT, typename _SuperT>
    class wm_killfocus : public _SuperT{

    protected:

      virtual void on_wm_killfocus(HWND) = 0{}

      explicit wm_killfocus(iwindow * pParent) : _SuperT(pParent){}

      virtual void handle_msg(window_message& msg) override{
        if (WM_KILLFOCUS == msg.umsg) on_wm_killfocus(reinterpret_cast<HWND>(msg.wparam));
      }
    };
  }
}