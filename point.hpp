#pragma once
namespace wtf{
  struct point : POINT{
    using vector = std::vector<point>;
    point(){ x = y = 0; }
    point(LONG X, LONG Y){ x = X; y = Y; }
    point(const point& src){ x = src.x; y = src.y; }
    point& operator=(const point& src){
      x = src.x;
      y = src.y;
      return *this;
    }
    bool is_in(const RECT& r) const{ return PtInRect(&r, point(*this)) ? true : false; }
  };
}