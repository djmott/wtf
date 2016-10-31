#pragma once

namespace wtf {

  template <coord_frame _frame> struct rect : RECT{
    using vector = std::vector<rect>;

    rect() = default;

    rect(LONG Left, LONG Top, LONG Right, LONG Bottom){
      left = Left;
      top = Top;
      right = Right;
      bottom = Bottom;
    }
    rect(const rect&) = default;
    rect(const RECT& src) : RECT(src){}
    rect& operator=(const rect&) = default;

    inline static rect get(HWND hwnd);
    point<_frame> position() const{ return point<_frame>(left, top); }
    point<_frame> dimensions() const{ return point<_frame>(right, bottom); }

    inline bool is_in(const point<_frame>& oPoint) const { 
      return (oPoint.x >= left && oPoint.y >= top && oPoint.x <= right && oPoint.y <= bottom);
    }

    rect& offset(const point<_frame>& oPoint){
      wtf::exception::throw_lasterr_if(::OffsetRect(this, -oPoint.x, -oPoint.y), [](BOOL b){ return !b; });
      return *this;
    }

  };

  template <>
  inline rect<coord_frame::client> rect<coord_frame::client>::get(HWND hwnd){
    rect<coord_frame::client> oRet;
    wtf::exception::throw_lasterr_if(::GetClientRect(hwnd, &oRet), [](BOOL b){ return !b; });
    return oRet;
  }

  template <>
  inline rect<coord_frame::screen> rect<coord_frame::screen>::get(HWND hwnd){
    rect<coord_frame::screen> oRet;
    wtf::exception::throw_lasterr_if(::GetWindowRect(hwnd, &oRet), [](BOOL b){ return !b; });
    return oRet;
  }


#if 0
    struct rect {

      template <typename _Ty, typename _PointT>
      struct base : RECT{
        using vector = std::vector<_Ty>;
        base() = default;
        base(const base&) = default;
        base& operator=(const base&) = default;

        base(const RECT &src){ memcpy(this, &src, sizeof(RECT)); }
        base& operator=(const RECT& src){
          memcpy(this, &src, sizeof(RECT));
          return *this;
        }

        base(LONG Left, LONG Top, LONG Right, LONG Bottom){
          left = Left;
          top = Top;
          right = Right;
          bottom = Bottom;
        }

        bool is_in(const _PointT & p) const{ return PtInRect(this, p) ? true : false; }
      
        _PointT position() const{ return _PointT(left, top); }
        _PointT dimensions() const{ return _PointT(right, bottom); }
      };


      struct screen_coords : base<screen_coords, point<coord_frame::screen>>{
        template <typename ... _Ty> screen_coords(_Ty&&... src) : base(std::forward<_Ty>(src)...){}

        static screen_coords get(HWND hwnd){
          screen_coords oRet;
          wtf::exception::throw_lasterr_if(GetWindowRect(hwnd, &oRet), [](BOOL b){ return !b; });
          return oRet;
        }

      };

      struct client_coord : base<client_coord, point<coord_frame::client>>{
        template <typename ... _Ty> client_coord(_Ty&&... src) : base(std::forward<_Ty>(src)...){}

        static client_coord get(HWND hwnd){
          client_coord oRet;
          wtf::exception::throw_lasterr_if(GetClientRect(hwnd, &oRet), [](BOOL b){ return !b; });
          return oRet;
        }

      };



    };

  #endif
  }

