#pragma once

namespace wtf {

    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::wm_paint, _Policies...> : public window<_ImplT, _Policies...>{
      using __super_t = window<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window;
    public:

    protected:

      virtual void on_wm_paint(const device_context&, const paint_struct&){}

      explicit window(iwindow * pParent) : __super_t(pParent){}

      LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam){
        if (WM_PAINT == umsg){
          auto & dc = *reinterpret_cast<const device_context *>(wparam);
          auto & ps = *reinterpret_cast<const paint_struct *>(lparam);
          on_wm_paint(dc, ps);
        }
        return __super_t::handle_message(hwnd, umsg, wparam, lparam);
      }

    };

  }
