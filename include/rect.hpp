#pragma once

namespace wtf {

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


      struct screen_coords : base<screen_coords, point::screen_coords>{
        template <typename ... _Ty> screen_coords(_Ty&&... src) : base(std::forward<_Ty>(src)...){}

        static screen_coords get(HWND hwnd){
          screen_coords oRet;
          wtf::exception::throw_lasterr_if(GetWindowRect(hwnd, &oRet), [](BOOL b){ return !b; });
          return oRet;
        }

      };

      struct client_coord : base<client_coord, point::client_coords>{
        template <typename ... _Ty> client_coord(_Ty&&... src) : base(std::forward<_Ty>(src)...){}

        static client_coord get(HWND hwnd){
          client_coord oRet;
          wtf::exception::throw_lasterr_if(GetClientRect(hwnd, &oRet), [](BOOL b){ return !b; });
          return oRet;
        }

      };



    };


  }

