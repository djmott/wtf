#pragma once

namespace wtf{
  struct size : SIZE{
    size(){ cx = cy = 0; }
    size(int x, int y){
      cx = x;
      cy = y;
    }
  };
}