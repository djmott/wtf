#pragma once

namespace wtf{
  namespace policy{
    template <typename _SuperT>
    class has_invalidate : public _SuperT{

      
    public:

      void invalidate(bool EraseBackground = true){
	      if (_SuperT::_handle) {
          wtf::exception::throw_lasterr_if(
            ::InvalidateRect(_SuperT::_handle, nullptr, (EraseBackground ? TRUE : FALSE)),
                                           [](BOOL b){ return !b; });
        }
      }

    protected:
      explicit has_invalidate(iwindow * pParent) : _SuperT(pParent){}

    };
  }
}
