#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT, typename _ImplT>
    class wm_nccalcsize : public _SuperT{

      
    public:

      enum class activate_mode{
        active = WA_ACTIVE,
        click_active = WA_CLICKACTIVE,
        inactive = WA_INACTIVE,
      };

    protected:

      virtual LRESULT on_wm_nccalcsize(NCCALCSIZE_PARAMS *) = 0;
      virtual LRESULT on_wm_nccalcsize(RECT *){ return 0; }

      explicit wm_nccalcsize(iwindow * pParent) : _SuperT(pParent){}

      virtual void handle_msg(window_message& msg) override{
        if (WM_NCCALCSIZE == msg.umsg){
          msg.bhandled = true;
          if (msg.wparam) msg.lresult = on_wm_nccalcsize(reinterpret_cast<NCCALCSIZE_PARAMS*>(msg.lparam));
          else  msg.lresult = on_wm_nccalcsize(reinterpret_cast<RECT*>(msg.lparam));

        }
        _SuperT::handle_msg(msg);
      }

    };
  }
}