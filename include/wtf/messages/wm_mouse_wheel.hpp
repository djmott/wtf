#pragma once

namespace wtf{


    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::wm_mouse_wheel, _Policies...> : public window<_ImplT, _Policies...>{
      using __super_t = window<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window;
    public:

    protected:

      virtual void on_wm_mouse_wheel(int16_t /*delta*/, const mouse_msg_param&){}

      explicit window(iwindow * pParent) : __super_t(pParent){}

      LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam){
        if (WM_MOUSEWHEEL == umsg) on_wm_mouse_wheel(static_cast<int16_t>(HIWORD(wparam)), mouse_msg_param(wparam, lparam, mouse_msg_param::buttons::unspecified));
        return __super_t::handle_message(hwnd, umsg, wparam, lparam);
      }
    };
  }
