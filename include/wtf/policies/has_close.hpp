#pragma once

namespace wtf{
  /** has_close
  * Add members to close a UI element and events to capture it
  */
  namespace policy{
    template <typename _SuperT>
    class has_close : public _SuperT{

      
    public:

      void close(){ CloseWindow(*this); }


    protected:

      explicit has_close(iwindow * pParent) : _SuperT(pParent){}

    };
  }
}