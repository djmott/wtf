#pragma once

namespace wtf{
  struct paint_struct : PAINTSTRUCT{
    ~paint_struct(){ ::EndPaint(_hwnd, this); }

    explicit paint_struct(HWND hwnd) : _hwnd(hwnd){ wtf::exception::throw_lasterr_if(::BeginPaint(_hwnd, this), [](HDC dc){ return !dc; }); }

    paint_struct(const paint_struct&) = delete;

    rect client_area() const{ return rcPaint; }
  protected:
    HWND _hwnd;
  };
}