#pragma once

namespace wtf{
  /** has_show
  * Adds members to change the display state and capture display state changes
  */
  namespace policy{
    template <typename _SuperT>
    class has_show : public _SuperT{

      
    public:


      virtual void show(){ ::ShowWindow(*this, SW_SHOW); }

      virtual void hide(){ ::ShowWindow(*this, SW_HIDE); }

    protected:

      explicit has_show(iwindow * pParent) : _SuperT(pParent){}

    };
  }
}
