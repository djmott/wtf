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

      virtual void OnClose(bool& cancel_close){}

      has_close(window<void> * pParent) : _SuperT(pParent){}


      LRESULT handle_message(HWND , UINT umsg, WPARAM , LPARAM , bool & bHandled) {
        if (WM_CLOSE == umsg){
          OnClose(bHandled);
        }
        return 0;
      }
    };
  }
}
