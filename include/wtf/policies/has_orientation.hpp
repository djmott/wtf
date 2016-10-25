#pragma once
namespace wtf{

  enum class orientations{
    horizontal,
    vertical
  };

    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::has_orientation, _Policies...> : public window<_ImplT, _Policies...>{
      using __super_t = window<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window;
    public:


      virtual orientations orientation() const{ return _orientation; }
      virtual void orientation(orientations newval){
        _orientation = newval;
      }

    protected:
      explicit window(iwindow * pParent) : __super_t(pParent){}
      orientations _orientation = orientations::horizontal;
    };
  }
