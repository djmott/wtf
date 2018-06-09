/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {

  struct device_context : std::shared_ptr<HDC__> {
    using deleter_type = void(*)(HDC);

    ~device_context() {}

    operator HDC() const noexcept {
      assert(get());
      return get();
    }

    device_context(device_context&& src) noexcept : shared_ptr(std::move(src)) {}

    device_context &operator=(device_context &&src) noexcept {
      shared_ptr::swap(src);
      return *this;
    }

    device_context(const device_context&) = delete;
    device_context &operator=(const device_context &) = delete;

    static device_context get_client(HWND hwnd) {
      return device_context(wtf::exception::throw_lasterr_if(::GetDC(hwnd), [](HDC dc)noexcept { return !dc; }),
        [hwnd](HDC dc)noexcept { ::ReleaseDC(hwnd, dc); });
    }

    static device_context get_window(HWND hwnd) {
      return device_context(wtf::exception::throw_lasterr_if(::GetWindowDC(hwnd), [](HDC dc)noexcept { return !dc; }),
        [hwnd](HDC dc)noexcept { ::ReleaseDC(hwnd, dc); });
    }
    static device_context get_dcex(HWND hwnd, const region& rgn, DWORD flags) {
      auto hRegion = (HRGN)rgn;
      return device_context(wtf::exception::throw_lasterr_if(::GetDCEx(hwnd, hRegion, flags), [](HDC dc)noexcept { return !dc; }),
        [hwnd](HDC dc)noexcept { ::ReleaseDC(hwnd, dc); });
    }
    static device_context get_dcex(HWND hwnd, DWORD flags) {
      return device_context(wtf::exception::throw_lasterr_if(::GetDCEx(hwnd, nullptr, flags), [](HDC dc)noexcept { return !dc; }),
        [hwnd](HDC dc)noexcept { ::ReleaseDC(hwnd, dc); });
    }

    static device_context get_screen() {
      return device_context(wtf::exception::throw_lasterr_if(::GetDC(nullptr), [](HDC dc)noexcept { return !dc; }),
        [](HDC dc)noexcept { ::ReleaseDC(nullptr, dc); });
    }


    void draw_focus_rect(const rect<coord_frame::client>& area) const {
      wtf::exception::throw_lasterr_if(::DrawFocusRect(*this, &area), [](BOOL b)noexcept { return !b; });
    }


    void fill(const region &oRegion, const brush &oBrush) const {
      wtf::exception::throw_lasterr_if(::FillRgn(*this, oRegion, oBrush), [](BOOL b)noexcept { return !b; });
    }

    void fill(const rect<coord_frame::client> &oRect, const brush &oBrush) const {
      wtf::exception::throw_lasterr_if(::FillRect(*this, &oRect, oBrush), [](int i)noexcept { return !i; });
    }

    void fill(const point<coord_frame::client>::vector &oPoints, const pen &oPen, const brush &oBrush) const {
      select_object(oPen);
      select_object(oBrush);
      wtf::exception::throw_lasterr_if(::Polygon(*this, &oPoints[0], static_cast<int>(oPoints.size())),
        [](BOOL b)noexcept { return !b; });
    }

    void arch(const pen& oPen, bool clockwise, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const {
      select_object(oPen);
      wtf::exception::throw_lasterr_if(::SetArcDirection(*this, clockwise ? AD_CLOCKWISE : AD_COUNTERCLOCKWISE),
        [](BOOL b)noexcept { return !b; });
      wtf::exception::throw_lasterr_if(::Arc(*this, x1, y1, x2, y2, x3, y3, x4, y4),
        [](BOOL b)noexcept { return !b; });

    }

    void intersect_clip_rect(const rect<coord_frame::client>& oClient) const {
      wtf::exception::throw_lasterr_if(
        ::IntersectClipRect(*this, oClient.left, oClient.top, oClient.right, oClient.bottom),
        [](int i)noexcept { return ERROR == i; });
    }

    void select_clip_rgn(const region& rgn) const {
      wtf::exception::throw_lasterr_if(
        ::SelectClipRgn(*this, *rgn),
        [](int i)noexcept { return ERROR == i; });
    }

    size get_text_extent(const tstring &str) const {
      size ret;
      wtf::exception::throw_lasterr_if(::GetTextExtentPoint32(*this, str.c_str(), static_cast<int>(str.size()), &ret),
        [](BOOL b)noexcept { return !b; });
      return ret;
    }

    size get_text_extent(const TCHAR *str, int len) const {
      size ret;
      wtf::exception::throw_lasterr_if(::GetTextExtentPoint32(*this, str, len, &ret),
        [](BOOL b)noexcept { return !b; });
      return ret;
    }

    rgb text_color() const {
      return rgb(wtf::exception::throw_lasterr_if(::GetTextColor(*this), [](COLORREF c)noexcept { return CLR_INVALID == c; }));
    }

    void text_color(const rgb& newval) const {
      wtf::exception::throw_lasterr_if(::SetTextColor(*this, newval), [](COLORREF c)noexcept { return CLR_INVALID == c; });
    }

    void invert(const rect<coord_frame::client> &area) const {
      wtf::exception::throw_lasterr_if(::InvertRect(*this, &area), [](BOOL b)noexcept { return !b; });
    }

    void line(const pen &oPen, int x1, int y1, int x2, int y2) const {
      select_object(oPen);
      wtf::exception::throw_lasterr_if(::MoveToEx(*this, x1, y1, nullptr), [](BOOL b)noexcept { return !b; });
      wtf::exception::throw_lasterr_if(::LineTo(*this, x2, y2), [](BOOL b)noexcept { return !b; });
    }

    void line(const pen &oPen, const point<coord_frame::client>& p1, const point<coord_frame::client>& p2) const {
      select_object(oPen);
      wtf::exception::throw_lasterr_if(::MoveToEx(*this, p1.x, p1.y, nullptr), [](BOOL b)noexcept { return !b; });
      wtf::exception::throw_lasterr_if(::LineTo(*this, p2.x, p2.y), [](BOOL b)noexcept { return !b; });
    }

    void line(const pen &oPen, const point<coord_frame::client>::vector& oPoints) const {
      select_object(oPen);
      wtf::exception::throw_lasterr_if(::Polyline(*this, &oPoints[0], static_cast<int>(oPoints.size())), [](BOOL b)noexcept { return !b; });
    }

    void move_to(int x, int y) const {
      wtf::exception::throw_lasterr_if(::MoveToEx(*this, x, y, nullptr), [](BOOL b)noexcept { return !b; });
    }

    void select_object(const brush &oBrush) const {
      wtf::exception::throw_lasterr_if(::SelectObject(*this, oBrush), [](HGDIOBJ o)noexcept { return !o || HGDI_ERROR == o; });
    }

    void select_object(const font::handle &oFont) const {
      wtf::exception::throw_lasterr_if(::SelectObject(*this, oFont), [](HGDIOBJ o)noexcept { return !o || HGDI_ERROR == o; });
    }

    void select_object(const pen &oPen) const {
      wtf::exception::throw_lasterr_if(::SelectObject(*this, oPen), [](HGDIOBJ o)noexcept { return !o || HGDI_ERROR == o; });
    }

    void select_object(const region &oRegion) const {
      wtf::exception::throw_lasterr_if(::SelectObject(*this, oRegion), [](HGDIOBJ o)noexcept { return !o || HGDI_ERROR == o; });
    }

  protected:
    template<typename ... _ArgTs> device_context(_ArgTs &&...oArgs) :
      shared_ptr(std::forward<_ArgTs>(oArgs)...) {}
  };
}