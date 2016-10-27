#pragma once

namespace wtf {

    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::wm_setcursor, _Policies...> 
      : public window_impl<_ImplT, _Policies...>
    {
      using __super_t = window_impl<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window_impl;
    public:


    protected:

      virtual void on_wm_setcursor(wm_nchittest_flags) = 0{}

      explicit window(iwindow * pParent) : __super_t(pParent){}

      virtual void handle_msg(window_message& msg) override{
        if (WM_SETCURSOR == msg.umsg) {
          on_wm_setcursor(static_cast<wm_nchittest_flags>(LOWORD(msg.lparam)));
          msg.lresult = TRUE;
          msg.bhandled = true;
        }
      }
    };
  }
