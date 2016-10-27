#pragma once

namespace wtf {

    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::wm_timer, _Policies...> 
      : public window_impl<_ImplT, _Policies...>
    {
      using __super_t = window_impl<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window_impl;
    public:


    protected:
      explicit window(iwindow * pParent) : __super_t(pParent){}

      virtual void on_wm_timer(UINT_PTR){}

      virtual void handle_msg(window_message& msg) override{
        if (WM_TIMER == msg.umsg) on_wm_timer(static_cast<UINT_PTR>(msg.wparam));
      }

    };

  }
