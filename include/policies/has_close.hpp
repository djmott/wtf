#pragma once

namespace wtf {
  namespace policy {
    /** has_close
    * Add members to close a UI element and events to capture it
    */
    template<typename _SuperT>
    struct has_close : _SuperT {
      void close() { CloseWindow(*this); }
      has_close() = default;
      has_close(const has_close&) = delete;
      has_close &operator=(const has_close &) = delete;
      has_close(has_close&&) = delete;
      has_close &operator=(has_close&&) = delete;
      callback<void()> CloseEvent;
    protected:
      virtual LRESULT handle_message(HWND , UINT umsg, WPARAM , LPARAM , bool &) override {
        if (WM_CLOSE == umsg) CloseEvent();
        return 0;
      }
    };
  }
}
