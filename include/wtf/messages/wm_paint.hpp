#pragma once

namespace wtf {

    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::wm_paint, _Policies...> 
      : public window_impl<_ImplT, _Policies...>
    {
      using __super_t = window_impl<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window_impl;
    public:

    protected:

      virtual void on_wm_paint(const device_context&, const paint_struct&){}

      explicit window(iwindow * pParent) : __super_t(pParent){}

      virtual void handle_msg(window_message& msg) override{
        if (WM_PAINT == msg.umsg){
          auto & dc = *reinterpret_cast<const device_context *>(msg.wparam);
          auto & ps = *reinterpret_cast<const paint_struct *>(msg.lparam);
          on_wm_paint(dc, ps);
        }
      }

    };

  }
