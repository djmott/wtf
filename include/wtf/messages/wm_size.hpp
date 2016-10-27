#pragma once

namespace wtf {

    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::wm_size, _Policies...> 
      : public window_impl<_ImplT, _Policies...>
    {
      using __super_t = window_impl<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window_impl;
    public:


    protected:

      explicit window(iwindow * pParent) : __super_t(pParent){}

      virtual void on_wm_size(const point<coord_frame::client>&){}

      virtual void handle_msg(window_message& msg) override{
        if (WM_SIZE == msg.umsg) on_wm_size( point<coord_frame::client>(LOWORD(msg.lparam), HIWORD(msg.lparam)));
      }

    };

  }
