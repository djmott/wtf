#pragma once
namespace wtf{
  namespace policy{
    template<typename _SuperT, typename>
    struct has_orientation: _SuperT{

      enum class orientations{
        horizontal,
        vertical
      };

      virtual orientations orientation() const{ return _orientation; }
      virtual void orientation(orientations newval){
        _orientation = newval;
      }

    protected:
      has_orientation(window<void> * pParent) : _SuperT(pParent){}
      orientations _orientation = orientations::horizontal;
    };
  }
}