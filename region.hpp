#pragma once

namespace wtf{


  struct const_region{
    virtual ~const_region() = default;
    HRGN operator*() const{ return _hrgn; }
    operator HRGN() const{ return _hrgn; }
    const_region(const const_region&) = delete;
    const_region(const_region&& src) : _hrgn(nullptr){
      std::swap(_hrgn, src._hrgn);
    }
  protected:
    const_region(HRGN newval) : _hrgn(newval){}
    HRGN _hrgn;
  };


  struct region : const_region{

    virtual ~region() override{ if (_hrgn) DeleteObject(_hrgn); }

    region(region&& src) : const_region(std::move(src)){}

    static region get(HWND hWnd){
      region oRet(wtf::exception::throw_lasterr_if(CreateRectRgn(0, 0, 0, 0), [](HRGN h){ return !h; }));
      GetWindowRgn(hWnd, *oRet);
      return oRet;
    }

  protected:
    region(HRGN newval) : const_region(newval){}
  };


}