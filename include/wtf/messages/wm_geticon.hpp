#pragma once

namespace wtf {

    enum class icon_type{
      big_icon = ICON_BIG,
      small_icon = ICON_SMALL,
      small2_icon = ICON_SMALL2,
    };


    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::wm_geticon, _Policies...> : public window<_ImplT, _Policies...>{
      using __super_t = window<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window;
    public:


    protected:

      virtual HICON on_wm_geticon(icon_type) = 0;

      explicit window(iwindow * pParent) : __super_t(pParent){}

      LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam){
        if (WM_GETICON == umsg) return reinterpret_cast<LRESULT>(on_wm_geticon(static_cast<icon_type>(wparam)));
        return __super_t::handle_message(hwnd, umsg, wparam, lparam);
      }
    };
}
