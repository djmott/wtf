#pragma once

namespace wtf {
  namespace policy {
    /** has_show
    * Adds members to change the display state and capture display state changes
    */
    template<typename _SuperT, typename _ImplT>
    struct has_show : _SuperT {

      virtual void show() { ::ShowWindow(*this, SW_SHOW); }

      virtual void hide() { ::ShowWindow(*this, SW_HIDE); }

    protected:

      explicit has_show(window<void,void> * pParent) : _SuperT(pParent){}

    };

  }
}
