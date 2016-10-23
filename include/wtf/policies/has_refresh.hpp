#if 0
#pragma once

namespace wtf{
  namespace policy{
    template <typename _SuperT, typename _ImplT> struct has_refresh : _SuperT{

      void refresh(bool EraseBackground = true){
        wtf::exception::throw_lasterr_if(
          ::InvalidateRect(*this, nullptr, (EraseBackground ? TRUE : FALSE)), [](BOOL b){ return !b; });
      }

    protected:
      has_refresh(window<void,void> * pParent) : _SuperT(pParent){}

    };
  }
}
#endif
