#pragma once
namespace wtf{

  enum class orientations{
    horizontal,
    vertical
  };
  namespace policy{
    template <typename _ImplT, typename _SuperT>
    class has_orientation : public _SuperT{

      
    public:


      virtual orientations orientation() const{ return _orientation; }
      virtual void orientation(orientations newval){
        _orientation = newval;
      }

    protected:
      explicit has_orientation(iwindow * pParent) : _SuperT(pParent){}
      orientations _orientation = orientations::horizontal;
    };
  }
}
