#pragma once

namespace wtf {

    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::wm_showwindow, _Policies...> 
      : public window_impl<_ImplT, _Policies...>
    {
      using __super_t = window_impl<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window_impl;
    public:

      enum class visibility_change_flag {
        show_window = 0,
        other_unzoom = SW_OTHERUNZOOM,
        other_zoom = SW_OTHERZOOM,
        parent_closing = SW_PARENTCLOSING,
        parent_opening = SW_PARENTOPENING,
      };

    protected:

      explicit window(iwindow * pParent) : __super_t(pParent){}

      virtual void on_wm_showwindow(visibility_change_flag) = 0{}

      virtual void handle_msg(window_message& msg) override{
        if (WM_SHOWWINDOW == msg.umsg) on_wm_showwindow(static_cast<visibility_change_flag>(msg.lparam));
      }
    };

  }
