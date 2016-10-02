#pragma once

namespace wtf{

  struct device_context : std::unique_ptr<HDC__, void(*)(HDC)>{


    ~device_context() = default;

    operator HDC() const{ assert(get()); return get(); }
    
    device_context(device_context&& src) : unique_ptr(std::move(src)){}

    device_context() : unique_ptr(nullptr, [](HDC){}){}

    device_context& operator=(device_context&& src){
      unique_ptr::swap(std::move(src));
      return *this;
    }

    static device_context get_client(HWND hwnd){
      return device_context(wtf::exception::throw_lasterr_if(::GetDC(hwnd), [](HDC dc){ return !dc; }), [](HDC dc){ ::ReleaseDC(nullptr, dc); });
    }

    static device_context get_window(HWND hwnd){
      return device_context(wtf::exception::throw_lasterr_if(::GetWindowDC(hwnd), [](HDC dc){ return !dc; }), [](HDC dc){ ::ReleaseDC(nullptr, dc); });
    }

    static device_context get_screen(){
      return device_context(wtf::exception::throw_lasterr_if(::GetDC(nullptr), [](HDC dc){ return !dc; }), [](HDC dc){ ::ReleaseDC(nullptr, dc); });
    }

    void line(const wtf::pen& oPen, int x1, int y1, int x2, int y2) const{
      SelectObject(*this, oPen);
      wtf::exception::throw_lasterr_if(MoveToEx(*this, x1, y1, nullptr), [](BOOL b){ return !b; });
      wtf::exception::throw_lasterr_if(LineTo(*this, x2, y2), [](BOOL b){ return !b; });
    }

    void fill(const region& oRegion, const brush& oBrush) const{ FillRgn(*this, oRegion, oBrush); }

    void fill(const rect& oRect, const brush& oBrush) const{ wtf::exception::throw_lasterr_if(FillRect(*this, &oRect, oBrush), [](int i){ return !i; }); }

    void fill(const point::vector& oPoints, const pen& oPen, const brush& oBrush) const{
      auto oldPen = SelectObject(*this, oPen);
      auto oldBrush = SelectObject(*this, oBrush);
      wtf::exception::throw_lasterr_if(Polygon(*this, &oPoints[0], static_cast<int>(oPoints.size())), [](BOOL b){return !b; });
      SelectObject(*this, oldBrush);
      SelectObject(*this, oldPen);
    }

  protected:
    template <typename ... _ArgTs> device_context(_ArgTs&&...oArgs) : unique_ptr(std::forward<_ArgTs>(oArgs)...){}

  };

}