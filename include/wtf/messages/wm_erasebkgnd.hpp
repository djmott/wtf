#if 0
#pragma once

namespace wtf {

    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::wm_erasebkgnd, _Policies...> : public window<_ImplT, _Policies...>{
      using __super_t = window<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window;
    public:

    protected:

      virtual void on_wm_erasebkgnd(const device_context&, const rect<coord_frame::client>&) = 0;

      explicit window(iwindow * pParent) : __super_t(pParent){}

      LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam){
        if (WM_ERASEBKGND == umsg){
          auto &oDC = *reinterpret_cast<const device_context *>(lparam);
          on_wm_erasebkgnd(oDC, rect<coord_frame::client>::get(*this));
        }
        return __super_t::handle_message(hwnd, umsg, wparam, lparam);
      }

    };

}
#endif