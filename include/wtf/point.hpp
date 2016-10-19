#pragma once

namespace wtf {

  template <coord_frame _frame> struct point : POINT{
    using vector = std::vector<point>;
    point(){ x = y = 0; }
    point(LONG X, LONG Y){ x = X; y = Y; }    
  };

#if 0
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
        static screen_coords get_size(HWND hwnd){
          RECT r;
          wtf::exception::throw_lasterr_if(::GetWindowRect(hwnd, &r), [](BOOL b){ return !b; });
          return screen_coords(r.right - r.left, r.bottom - r.top);
        }
      };

      struct client_coords : base<screen_coords>{
        client_coords() = default;
        client_coords(LONG X, LONG Y) : base(X,Y){}
        screen_coords to_screen(HWND hwnd) const {
          screen_coords oRet;
          wtf::exception::throw_lasterr_if(::ClientToScreen(hwnd, &oRet), [](BOOL b){ return !b; });
          return oRet;
        }
        static client_coords get_size(HWND hwnd){
          RECT r;
          wtf::exception::throw_lasterr_if(::GetClientRect(hwnd, &r), [](BOOL b){ return !b; });
          return client_coords(r.right - r.left, r.bottom - r.top);
        }
      };

    };


    inline point<coord_frame::client> point<coord_frame::screen>::to_client(HWND hwnd) const{
      client_coords oRet;
      wtf::exception::throw_lasterr_if(::ScreenToClient(hwnd, &oRet), [](BOOL b){ return !b; });
      return oRet;
    }
  #endif
  }

