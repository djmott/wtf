#pragma once

namespace wtf{

  struct device_context : std::unique_ptr<HDC__, std::function < void(HDC)>>{
    using deleter_type = void(*)(HDC);

    ~device_context(){}

    operator HDC() const{
      assert(get());
      return get();
    }

    device_context(device_context&& src) :

      unique_ptr(std::move(src)){}

    device_context &operator=(device_context &&src){
      unique_ptr::swap(src);
      return *this;
    }

    device_context(const device_context&) = delete;
    device_context &operator=(const device_context &) = delete;

    static device_context get_client(HWND hwnd){
      return device_context(wtf::exception::throw_lasterr_if(::GetDC(hwnd), [](HDC dc){ return !dc; }),
                            [hwnd](HDC dc){ ::ReleaseDC(hwnd, dc); });
    }

    static device_context get_window(HWND hwnd){
      return device_context(wtf::exception::throw_lasterr_if(::GetWindowDC(hwnd), [](HDC dc){ return !dc; }),
                            [hwnd](HDC dc){ ::ReleaseDC(hwnd, dc); });
    }
    static device_context get_dcex(HWND hwnd, region& rgn, DWORD flags){
      return device_context(wtf::exception::throw_lasterr_if(::GetDCEx(hwnd, rgn, flags), [](HDC dc){ return !dc; }),
                            [hwnd](HDC dc){ ::ReleaseDC(hwnd, dc); });
    }

    static device_context get_screen(){
      return device_context(wtf::exception::throw_lasterr_if(::GetDC(nullptr), [](HDC dc){ return !dc; }),
                            [](HDC dc){ ::ReleaseDC(nullptr, dc); });
    }


    void draw_focus_rect(const rect::client_coord& area) const{
      wtf::exception::throw_lasterr_if(::DrawFocusRect(*this, &area), [](BOOL b){ return !b; });
    }


    void fill(const region &oRegion, const brush &oBrush) const{
      wtf::exception::throw_lasterr_if(::FillRgn(*this, oRegion, oBrush), [](BOOL b){ return !b; });
    }

    void fill(const rect::client_coord &oRect, const brush &oBrush) const{
      wtf::exception::throw_lasterr_if(::FillRect(*this, &oRect, oBrush), [](int i){ return !i; });
    }

    void fill(const point::client_coords::vector &oPoints, const pen &oPen, const brush &oBrush) const{
      select_object(oPen);
      select_object(oBrush);
      wtf::exception::throw_lasterr_if(::Polygon(*this, &oPoints[0], static_cast<int>(oPoints.size())),
                                       [](BOOL b){ return !b; });
    }

    void arch(const pen& oPen, bool clockwise, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const{
      select_object(oPen);
      wtf::exception::throw_lasterr_if(::SetArcDirection(*this, clockwise ? AD_CLOCKWISE : AD_COUNTERCLOCKWISE),
                                       [](BOOL b){ return !b; });
      wtf::exception::throw_lasterr_if(::Arc(*this, x1, y1, x2, y2, x3, y3, x4, y4),
                                       [](BOOL b){ return !b; });
      
    }

    size get_text_extent(const tstring &str) const{
      size ret;
      wtf::exception::throw_lasterr_if(::GetTextExtentPoint32(*this, str.c_str(), static_cast<int>(str.size()), &ret),
                                       [](BOOL b){ return !b; });
      return ret;
    }

    size get_text_extent(const TCHAR *str, int len) const{
      size ret;
      wtf::exception::throw_lasterr_if(::GetTextExtentPoint32(*this, str, len, &ret),
                                       [](BOOL b){ return !b; });
      return ret;
    }

    rgb text_color() const{
      return rgb(wtf::exception::throw_lasterr_if(::GetTextColor(*this), [](COLORREF c){ return CLR_INVALID == c; }));
    }

    void text_color(const rgb& newval) const{
      wtf::exception::throw_lasterr_if(::SetTextColor(*this, newval), [](COLORREF c){ return CLR_INVALID == c; });
    }

    void invert(const rect::client_coord &area) const{
      wtf::exception::throw_lasterr_if(::InvertRect(*this, &area), [](BOOL b){ return !b; });
    }

    void line(const pen &oPen, int x1, int y1, int x2, int y2) const{
      select_object(oPen);
      wtf::exception::throw_lasterr_if(::MoveToEx(*this, x1, y1, nullptr), [](BOOL b){ return !b; });
      wtf::exception::throw_lasterr_if(::LineTo(*this, x2, y2), [](BOOL b){ return !b; });
    }

    void move_to(int x, int y) const{
      wtf::exception::throw_lasterr_if(::MoveToEx(*this, x, y, nullptr), [](BOOL b){ return !b; });
    }

    void select_object(const brush &oBrush) const{
      wtf::exception::throw_lasterr_if(::SelectObject(*this, oBrush), [](HGDIOBJ o){ return !o || HGDI_ERROR == o; });
    }

    void select_object(const font::handle &oFont) const{
      wtf::exception::throw_lasterr_if(::SelectObject(*this, oFont), [](HGDIOBJ o){ return !o || HGDI_ERROR == o; });
    }

    void select_object(const pen &oPen) const{
      wtf::exception::throw_lasterr_if(::SelectObject(*this, oPen), [](HGDIOBJ o){ return !o || HGDI_ERROR == o; });
    }

    void select_object(const region &oRegion) const{
      wtf::exception::throw_lasterr_if(::SelectObject(*this, oRegion), [](HGDIOBJ o){ return !o || HGDI_ERROR == o; });
    }

  protected:
    template<typename ... _ArgTs> device_context(_ArgTs &&...oArgs) :
      unique_ptr(std::forward<_ArgTs>(oArgs)...){}
  };
}
