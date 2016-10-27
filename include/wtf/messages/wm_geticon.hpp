#pragma once

namespace wtf {

    enum class icon_type{
      big_icon = ICON_BIG,
      small_icon = ICON_SMALL,
      small2_icon = ICON_SMALL2,
    };


    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::wm_geticon, _Policies...> 
      : public window_impl<_ImplT, _Policies...>
    {
      using __super_t = window_impl<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window_impl;
    public:


    protected:

      virtual HICON on_wm_geticon(icon_type) = 0;

      explicit window(iwindow * pParent) : __super_t(pParent){}

      virtual void handle_msg(window_message& msg) override{
        if (WM_GETICON == msg.umsg){
          msg.lresult = reinterpret_cast<LRESULT>(on_wm_geticon(static_cast<icon_type>(msg.wparam)));
          msg.bhandled = true;
        }
      }
    };
}
