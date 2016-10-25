#pragma once

namespace wtf{

  template <typename _ImplT, policy..._Policies>
  class window<_ImplT, policy::wm_dblclick, _Policies...> : public window<_ImplT, _Policies...>{
    using __super_t = window<_ImplT, _Policies...>;
    template <typename, policy ... > friend class window;
  public:


  protected:
    virtual void on_wm_dblclick(const mouse_msg_param&){}

    explicit window(iwindow * pParent) : __super_t(pParent){}

    LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam){
      if (WM_LBUTTONDBLCLK == umsg){
        on_wm_dblclick(mouse_msg_param(wparam, lparam, mouse_msg_param::buttons::left));
      } else if (WM_MBUTTONDBLCLK == umsg){
        on_wm_dblclick(mouse_msg_param(wparam, lparam, mouse_msg_param::buttons::middle));
      } else if (WM_RBUTTONDBLCLK == umsg){
        on_wm_dblclick(mouse_msg_param(wparam, lparam, mouse_msg_param::buttons::right));
      }
      return __super_t::handle_message(hwnd, umsg, wparam, lparam);
    }
  };
}
