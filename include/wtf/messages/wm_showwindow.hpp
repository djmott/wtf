#pragma once

namespace wtf {
  namespace policy {

    template<typename _SuperT, typename _ImplT>
    struct wm_showwindow : _SuperT {

      enum class visibility_change_flag {
        show_window = 0,
        other_unzoom = SW_OTHERUNZOOM,
        other_zoom = SW_OTHERZOOM,
        parent_closing = SW_PARENTCLOSING,
        parent_opening = SW_PARENTOPENING,
      };

    protected:

      wm_showwindow(window<void> * pParent) : _SuperT(pParent){}

      virtual LRESULT on_wm_showwindow(visibility_change_flag, bool&) = 0;

      LRESULT handle_message(HWND , UINT umsg, WPARAM , LPARAM lparam, bool & bHandled) {
        if (WM_SHOWWINDOW == umsg) return on_wm_showwindow(static_cast<visibility_change_flag>(lparam), bHandled);
        return 0;
      }
    };

  }
}
