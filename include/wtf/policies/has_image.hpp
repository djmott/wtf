#pragma once

namespace wtf{
  namespace policy{
    template <typename _ImplT, typename _SuperT>
    class has_image : public _SuperT{

      
    public:

    protected:
      explicit has_image(iwindow * pParent) : _SuperT(pParent){}
    };
  }
}
