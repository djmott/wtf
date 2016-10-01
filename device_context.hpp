#pragma once

namespace wtf{

  struct device_context{

    virtual ~device_context(){ if (_hwnd && _dc) ReleaseDC(_hwnd, _dc); }
    explicit device_context(HWND hwnd) : _hwnd(hwnd), _dc(GetDC(_hwnd)){}
    explicit device_context(HDC hdc) : _hwnd(nullptr), _dc(hdc){}
    device_context(device_context&& src) : _dc(std::move(src._dc)){}
    device_context& operator=(device_context&& src){
      std::swap(_dc, src._dc);
      std::swap(_hwnd, src._hwnd);
      return *this;
    }

    HDC operator*() const{ return _dc; }
    operator HDC() const{ return _dc; }

    void line(const wtf::pen& oPen, int x1, int y1, int x2, int y2) const{
      SelectObject(_dc, *oPen);
      wtf::exception::throw_lasterr_if(MoveToEx(_dc, x1, y1, nullptr), [](BOOL b){ return !b; });
      wtf::exception::throw_lasterr_if(LineTo(_dc, x2, y2), [](BOOL b){ return !b; });
    }

    void fill(const const_region& oRegion, const const_brush& oBrush){ FillRgn(_dc, *oRegion, *oBrush); }

    void fill(const rect& oRect, const const_brush& oBrush) const{ wtf::exception::throw_lasterr_if(FillRect(_dc, &oRect, *oBrush), [](int i){ return !i; }); }

  protected:
    HWND _hwnd;
    HDC _dc;
  };

}