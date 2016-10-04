#pragma once

namespace wtf {
  namespace policy {
    /** has_show
    * Adds members to change the display state and capture display state changes
    */
    template<typename _SuperT>
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
      callback<void(visibility_change_flag)> VisibilityChangedEvent;

    protected:
      virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool &bhandled) override {
        if (WM_SHOWWINDOW == umsg) VisibilityChangedEvent(static_cast<visibility_change_flag>(lparam));
        return 0;
      }
    };

  }
}
