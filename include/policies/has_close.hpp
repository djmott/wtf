#pragma once

namespace wtf {
  namespace policy {
    /** has_close
    * Add members to close a UI element and events to capture it
    */
    template<typename _SuperT, typename _ImplT>
    struct has_close : _SuperT {
      void close() { CloseWindow(*this); }
      has_close() = default;
      has_close(const has_close&) = delete;
      has_close &operator=(const has_close &) = delete;
      has_close(has_close&&) = delete;
      has_close &operator=(has_close&&) = delete;

    protected:

      virtual void CloseEvent(bool& /*cancel*/){}

      virtual LRESULT handle_message(HWND , UINT umsg, WPARAM , LPARAM , bool & bHandled) override {
        if (WM_CLOSE == umsg){
          CloseEvent(bHandled);
        }
        return 0;
      }
    };
  }
}
