#pragma once

namespace wtf{
  namespace policy{

    template <typename _SuperT, typename _ImplT>
    class wm_activate : public _SuperT{


      virtual void handle_msg(window_message& msg) override{
        if (WM_ACTIVATE == msg.umsg) on_wm_activate(static_cast<activate_mode>(LOWORD(msg.wparam)), HIWORD(msg.wparam) ? true : false, reinterpret_cast<HWND>(msg.lparam));
        _SuperT::handle_msg(msg);
      }

    public:
      enum class activate_mode{
        active = WA_ACTIVE,
        click_active = WA_CLICKACTIVE,
        inactive = WA_INACTIVE,
      };

    protected:

      virtual void on_wm_activate(activate_mode, bool minimized, HWND target){}

      explicit wm_activate(iwindow * pParent) : _SuperT(pParent){}

    };

  }
}