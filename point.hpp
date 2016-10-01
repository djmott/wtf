#pragma once
namespace wtf{

  struct point : POINT{
    point(){ x = y = 0; }
    point(LONG X, LONG Y){ x = X; y = Y; }
    point(const point& src){ x = src.x; y = src.y; }
    point& operator=(const point& src){
      x = src.x;
      y = src.y;
      return *this;
    }
  };

}