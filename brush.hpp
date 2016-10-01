#pragma once
namespace wtf{

  struct const_brush{

    virtual ~const_brush() = default;

    virtual const_brush clone() const{ return const_brush(_hbrush); }

    const_brush(const const_brush&) = delete;

    const_brush(const_brush&& src) : _hbrush(nullptr){
      std::swap(_hbrush, src._hbrush);
    }

    HBRUSH operator*() const{ return _hbrush; }
    operator HBRUSH() const{ return _hbrush; }

  protected:
    const_brush(HBRUSH newval) : _hbrush(newval){}
    HBRUSH _hbrush;
  };


  template <system_colors _ID>
  struct system_brush : const_brush{
    virtual ~system_brush() override{}

    virtual const_brush clone() const override{ return system_brush(); }

    system_brush() : const_brush(GetSysColorBrush(static_cast<int>(_ID))){}

  };


  template <COLORREF _Color>
  struct solid_brush : const_brush{
    solid_brush(solid_brush&& src) : const_brush(std::move(src)){}
    solid_brush() : const_brush(wtf::exception::throw_lasterr_if(CreateSolidBrush(_Color), [](HBRUSH h){ return !h; })){}
    virtual ~solid_brush() override{ DeleteObject(_hbrush); }
    virtual const_brush clone() const override{ return solid_brush(); }

  };

  //these brushes should be used in the window class during window creation instead of system brushes
  template <system_colors _ID>
  struct create_window_system_brush : const_brush{
    create_window_system_brush() : const_brush(reinterpret_cast<HBRUSH>(1+ static_cast<int>(_ID))){}
    virtual const_brush clone() const override{ return create_window_system_brush(); }
  };

  namespace brushes{

    struct null_brush : const_brush{
      null_brush() : const_brush((HBRUSH)nullptr){}
      virtual const_brush clone() const override{ return null_brush(); }
    };

  }

}