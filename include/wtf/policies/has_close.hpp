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

      explicit has_close(window<void,void> * pParent) : _SuperT(pParent){}

    };
  }
}
