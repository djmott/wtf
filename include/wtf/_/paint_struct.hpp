#pragma once

namespace wtf {
  struct paint_struct : private PAINTSTRUCT {
    ~paint_struct() { ::EndPaint(_hwnd, this); }

    explicit paint_struct(HWND hwnd) : _hwnd(hwnd) {
      wtf::exception::throw_lasterr_if(::BeginPaint(_hwnd, this), [](HDC dc) { return !dc; });
    }

    paint_struct(const paint_struct &) = delete;

    rect<coord_frame::client>& client(){ return *static_cast<rect<coord_frame::client>*>(&rcPaint); }
    const rect<coord_frame::client>& client() const { return *static_cast<const rect<coord_frame::client>*>(&rcPaint); }

  protected:
    HWND _hwnd;
  };
}
