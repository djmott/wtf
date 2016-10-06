#pragma once

namespace wtf {

    struct point {

      template <typename _Ty>
      struct base : POINT{
        base(){ x = y = 0; }
        base(LONG X, LONG Y){ x = X; y = Y; }
        using vector = std::vector<base>;
        bool is_in(const RECT &r) const{ return PtInRect(&r, base(*this)) ? true : false; }
      };

      struct client_coords;

      struct screen_coords : base<screen_coords>{
        screen_coords() = default;
        screen_coords(LONG X, LONG Y) : base(X, Y){}
        inline client_coords to_client(HWND) const ;
      };
      struct client_coords : base<screen_coords>{
        client_coords() = default;
        client_coords(LONG X, LONG Y) : base(X,Y){}
        screen_coords to_screen(HWND hwnd) const {
          screen_coords oRet;
          wtf::exception::throw_lasterr_if(::ClientToScreen(hwnd, &oRet), [](BOOL b){ return !b; });
          return oRet;
        }
      };

    };


    inline point::client_coords point::screen_coords::to_client(HWND hwnd) const{
      client_coords oRet;
      wtf::exception::throw_lasterr_if(::ScreenToClient(hwnd, &oRet), [](BOOL b){ return !b; });
      return oRet;
    }

  }

