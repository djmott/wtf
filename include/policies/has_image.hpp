#pragma once

namespace wtf{
  namespace policy{
    template <typename _SuperT, typename _ImplT> struct has_image : _SuperT{
    protected:
      has_image(window<void> * pParent) : _SuperT(pParent){}
    };
  }
}