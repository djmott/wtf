#pragma once
namespace wtf{

  enum class orientations{
    horizontal,
    vertical
  };

  namespace policy{
    template<typename _SuperT, typename>
    struct has_orientation: _SuperT{


      virtual orientations orientation() const{ return _orientation; }
      virtual void orientation(orientations newval){
        _orientation = newval;
      }

    protected:
      has_orientation(window<void,void> * pParent) : _SuperT(pParent){}
      orientations _orientation = orientations::horizontal;
    };
  }
}