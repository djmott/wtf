/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {

  struct paint_struct : PAINTSTRUCT {
    paint_struct() = default;
    ~paint_struct() { ::EndPaint(_hwnd, this); }

    explicit paint_struct(HWND hwnd) : _hwnd(hwnd) {
      wtf::exception::throw_lasterr_if(::BeginPaint(_hwnd, this), [](HDC dc)noexcept { return !dc; });
    }

    paint_struct(const paint_struct &) = delete;
    paint_struct& operator=(const paint_struct&) = delete;

    paint_struct(paint_struct&& src) noexcept : _hwnd(nullptr) { std::swap(_hwnd, src._hwnd); }
    paint_struct& operator=(paint_struct&& src) noexcept {
      std::swap(_hwnd, src._hwnd);
      return *this;
    }


    rect<coord_frame::client>& client() noexcept { return *static_cast<rect<coord_frame::client>*>(&rcPaint); }
    const rect<coord_frame::client>& client() const noexcept { return *static_cast<const rect<coord_frame::client>*>(&rcPaint); }

  protected:
    HWND _hwnd;
  };

}