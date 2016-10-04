#pragma once

namespace wtf {
  namespace policy {
    /** has_close
    * Add members to close a UI element and events to capture it
    */
    template<typename _SuperT>
    struct has_close : _SuperT {
      void close() { CloseWindow(*this); }

      callback<void()> CloseEvent;
    protected:
      virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool &bhandled) override {
        if (WM_CLOSE == umsg) CloseEvent();
        return 0;
      }
    };
  }
}
