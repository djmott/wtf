#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT>
    class wm_killfocus : public _SuperT{

    protected:

      virtual void on_wm_killfocus(HWND) {}

      explicit wm_killfocus(iwindow * pParent) : _SuperT(pParent){}

      void handle_msg(window_message& msg) override{
        if (WM_KILLFOCUS == msg.umsg) on_wm_killfocus(reinterpret_cast<HWND>(msg.wparam));
        _SuperT::handle_msg(msg);
      }
    };
  }
}