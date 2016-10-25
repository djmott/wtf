#pragma once

namespace wtf {

    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::wm_showwindow, _Policies...> : public window<_ImplT, _Policies...>{
      using __super_t = window<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window;
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

      virtual void on_wm_showwindow(visibility_change_flag){}

      LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam){
        if (WM_SHOWWINDOW == umsg) on_wm_showwindow(static_cast<visibility_change_flag>(lparam));
        return __super_t::handle_message(hwnd, umsg, wparam, lparam);
      }
    };

  }
