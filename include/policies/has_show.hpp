#pragma once

namespace wtf {
  namespace policy {
    /** has_show
    * Adds members to change the display state and capture display state changes
    */
    template<typename _SuperT>
    struct has_show : _SuperT {
      has_show() = default;
      ~has_show() = default;
      has_show(const has_show&) = delete;
      has_show &operator=(const has_show &) = delete;
      has_show(has_show&&) = delete;
      has_show &operator=(has_show&&) = delete;

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
