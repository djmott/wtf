﻿/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{

    /** @class has_cursor
    @brief Provides mouse pointer customization
    */
    template <typename _super_t>
    struct has_cursor : _super_t{

      virtual const wtf::cursor &cursor_pointer() const{ return cursor::global(cursor::style::arrow); }

      virtual const wtf::cursor &cursor_size_ns() const{ return cursor::global(cursor::style::size_ns); }

      virtual const wtf::cursor &cursor_size_we() const{ return cursor::global(cursor::style::size_we); }

      virtual const wtf::cursor &cursor_size_nwse() const{ return cursor::global(cursor::style::size_nwse); }

      virtual const wtf::cursor &cursor_size_nesw() const{ return cursor::global(cursor::style::size_nesw); }

    protected:

      void on_wm_setcursor(wm_nchittest_flags flags) override{
        switch (flags){
          case wm_nchittest_flags::top:
          case wm_nchittest_flags::bottom:
            SetCursor(cursor_size_ns());
            break;
          case wm_nchittest_flags::bottomleft:
          case wm_nchittest_flags::topright:
            SetCursor(cursor_size_nesw());
            break;
          case wm_nchittest_flags::bottomright:
          case wm_nchittest_flags::topleft:
            SetCursor(cursor_size_nwse());
            break;
          case wm_nchittest_flags::left:
          case wm_nchittest_flags::right:
            SetCursor(cursor_size_we());
            break;
          default:
            SetCursor(cursor_pointer());
            break;
        }
      }

    };
  }


}