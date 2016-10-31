#pragma once

namespace wtf{
  namespace policy{

    template <typename _SuperT>
    class wm_activate : public _SuperT{


    public:
      enum class activate_mode{
        active = WA_ACTIVE,
        click_active = WA_CLICKACTIVE,
        inactive = WA_INACTIVE,
      };

    protected:

      virtual void on_wm_activate(activate_mode, bool minimized, HWND target){}

      void handle_msg(window_message& msg) override{
        if (WM_ACTIVATE == msg.umsg) on_wm_activate(static_cast<activate_mode>(LOWORD(msg.wparam)), HIWORD(msg.wparam) ? true : false, reinterpret_cast<HWND>(msg.lparam));
        _SuperT::handle_msg(msg);
      }


      explicit wm_activate(iwindow * pParent) : _SuperT(pParent){}

    };

  }
}