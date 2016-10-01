#pragma once

namespace wtf{

  struct pen{
    HPEN operator*() const{ return _pen; }
    operator HPEN() const{ return _pen; }
    pen(pen&& src) : _pen(std::move(src._pen)){}
    virtual ~pen(){}
  protected:
    explicit pen(HPEN newval) : _pen(newval){}
    HPEN _pen;
  };

  template <int _width, typename _ColorT> struct solid_pen : pen{
    solid_pen() : pen(wtf::exception::throw_lasterr_if(CreatePen(PS_SOLID, _width, _ColorT()), [](HPEN p){ return !p; })){}
    virtual ~solid_pen() override{ DeleteObject(_pen); }
  };

}
