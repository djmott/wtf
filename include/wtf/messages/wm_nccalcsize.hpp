#pragma once

namespace wtf{

    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::wm_nccalcsize, _Policies...> 
      : public window_impl<_ImplT, _Policies...>
    {
      using __super_t = window_impl<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window_impl;
    public:

      enum class activate_mode{
        active = WA_ACTIVE,
        click_active = WA_CLICKACTIVE,
        inactive = WA_INACTIVE,
      };

    protected:

      virtual LRESULT on_wm_nccalcsize(NCCALCSIZE_PARAMS *) = 0;
      virtual LRESULT on_wm_nccalcsize(RECT *){ return 0; }

      explicit window(iwindow * pParent) : __super_t(pParent){}

      virtual void handle_msg(window_message& msg) override{
        if (WM_NCCALCSIZE == msg.umsg){
          msg.bhandled = true;
          if (msg.wparam) msg.lresult = on_wm_nccalcsize(reinterpret_cast<NCCALCSIZE_PARAMS*>(msg.lparam));
          else  msg.lresult = on_wm_nccalcsize(reinterpret_cast<RECT*>(msg.lparam));
        }
      }

    };
  }
