#pragma once

namespace wtf {
  namespace policy {
    /** has_close
    * Add members to close a UI element and events to capture it
    */
    template<typename _SuperT, typename _ImplT>
    struct has_close : _SuperT {

      void close() { CloseWindow(*this); }

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
