#pragma once

namespace wtf {
  namespace policy {
    /** has_show
    * Adds members to change the display state and capture display state changes
    */
    template<typename _SuperT, typename _ImplT>
    struct has_show : _SuperT {

      void show() { ::ShowWindow(*this, SW_SHOW); }

      void hide() { ::ShowWindow(*this, SW_HIDE); }

      enum class visibility_change_flag {
        show_window = 0,
        other_unzoom = SW_OTHERUNZOOM,
        other_zoom = SW_OTHERZOOM,
        parent_closing = SW_PARENTCLOSING,
        parent_opening = SW_PARENTOPENING,
      };

    protected:

      virtual void VisibilityChangedEvent(visibility_change_flag){}

      virtual LRESULT handle_message(HWND , UINT umsg, WPARAM , LPARAM lparam, bool &) override {
        if (WM_SHOWWINDOW == umsg) VisibilityChangedEvent(static_cast<visibility_change_flag>(lparam));
        return 0;
      }
    };

  }
}
