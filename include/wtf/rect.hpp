/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {

  template <coord_frame _frame> struct rect : RECT{
    using vector = std::vector<rect>;

    rect() = default;

    rect(LONG Left, LONG Top, LONG Right, LONG Bottom)noexcept {
      left = Left;
      top = Top;
      right = Right;
      bottom = Bottom;
    }
    rect(const rect&) = default;
    rect(const RECT& src) noexcept : RECT(src){}
    rect& operator=(const rect&) = default;

    inline static rect get(HWND hwnd);
    point<_frame> position() const noexcept { return point<_frame>(left, top); }
    point<_frame> dimensions() const noexcept { return point<_frame>(right, bottom); }

    inline bool is_in(const point<_frame>& oPoint) const noexcept {
      return (oPoint.x >= left && oPoint.y >= top && oPoint.x <= right && oPoint.y <= bottom);
    }

    rect& offset(const point<_frame>& oPoint) {
      wtf::exception::throw_lasterr_if(::OffsetRect(this, -oPoint.x, -oPoint.y), [](BOOL b)noexcept { return !b; });
      return *this;
    }

  };

  template <>
  inline rect<coord_frame::client> rect<coord_frame::client>::get(HWND hwnd) {
    rect<coord_frame::client> oRet;
    wtf::exception::throw_lasterr_if(::GetClientRect(hwnd, &oRet), [](BOOL b)noexcept { return !b; });
    return oRet;
  }

  template <>
  inline rect<coord_frame::screen> rect<coord_frame::screen>::get(HWND hwnd) {
    rect<coord_frame::screen> oRet;
    wtf::exception::throw_lasterr_if(::GetWindowRect(hwnd, &oRet), [](BOOL b)noexcept { return !b; });
    return oRet;
  }


  }

