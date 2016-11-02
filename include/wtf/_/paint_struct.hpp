/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace _{
    struct paint_struct : PAINTSTRUCT{
      ~paint_struct(){ ::EndPaint(_hwnd, this); }

      explicit paint_struct(HWND hwnd) : _hwnd(hwnd){
        wtf::exception::throw_lasterr_if(::BeginPaint(_hwnd, this), [](HDC dc){ return !dc; });
      }

      paint_struct(const _::paint_struct &) = delete;

      rect<coord_frame::client>& client(){ return *static_cast<rect<coord_frame::client>*>(&rcPaint); }
      const rect<coord_frame::client>& client() const{ return *static_cast<const rect<coord_frame::client>*>(&rcPaint); }

    protected:
      HWND _hwnd;
    };
  }
}