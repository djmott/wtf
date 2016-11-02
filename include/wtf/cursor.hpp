/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {

    struct cursor : std::shared_ptr<HICON__> {
      enum class style {
        arrow = 32512,
        ibeam = 32513,
        wait = 32514,
        cross = 32515,
        up_arrow = 32516,
        size_nwse = 32642,
        size_nesw = 32643,
        size_we = 32644,
        size_ns = 32645,
        size_all = 32646,
        no = 32648,
        hand = 32649,
        app_starting = 32650,
        help = 32651,
      };

      operator HCURSOR() const { return get(); }

      static cursor system(style Style) {
        return cursor(
          wtf::exception::throw_lasterr_if(::LoadCursor(nullptr, MAKEINTRESOURCE(Style)), [](HCURSOR h) { return !h; }),
          [](HCURSOR) {});
      }

      cursor(cursor &&src) : shared_ptr(std::move(src)) {}

      cursor &operator=(cursor &&src) {
        shared_ptr::swap(src);
        return *this;
      }

      cursor(const cursor&) = delete;
      cursor &operator=(const cursor &) = delete;


      static int show() { return ::ShowCursor(TRUE); }

      static int hide() { return ::ShowCursor(FALSE); }

      static void position(const point<coord_frame::screen> &p) {
        wtf::exception::throw_lasterr_if(::SetCursorPos(p.x, p.y), [](BOOL b) { return !b; });
      }

      static point<coord_frame::screen> position() {
        point<coord_frame::screen> oRet;
        wtf::exception::throw_lasterr_if(::GetCursorPos(&oRet), [](BOOL b) { return !b; });
        return oRet;
      }

      static void clip(const rect<coord_frame::screen> &area) {
        wtf::exception::throw_lasterr_if(::ClipCursor(&area), [](BOOL b) { return !b; });
      }

      static void unclip() { wtf::exception::throw_lasterr_if(::ClipCursor(nullptr), [](BOOL b) { return !b; }); }

      static const cursor &global(style Style) {
        static auto _arrow = system(style::arrow);
        static auto _ibeam = system(style::ibeam);
        static auto _wait = system(style::wait);
        static auto _cross = system(style::cross);
        static auto _up_arrow = system(style::up_arrow);
        static auto _size_nwse = system(style::size_nwse);
        static auto _size_nesw = system(style::size_nesw);
        static auto _size_we = system(style::size_we);
        static auto _size_ns = system(style::size_ns);
        static auto _size_all = system(style::size_all);
        static auto _no = system(style::no);
        static auto _hand = system(style::hand);
        static auto _app_starting = system(style::app_starting);
        static auto _help = system(style::help);
        switch (Style) {
          case (style::arrow):
            return _arrow;
          case (style::ibeam):
            return _ibeam;
          case (style::wait):
            return _wait;
          case (style::cross):
            return _cross;
          case (style::up_arrow):
            return _up_arrow;
          case (style::size_nwse):
            return _size_nwse;
          case (style::size_nesw):
            return _size_nesw;
          case (style::size_we):
            return _size_we;
          case (style::size_ns):
            return _size_ns;
          case (style::size_all):
            return _size_all;
          case (style::no):
            return _no;
          case (style::hand):
            return _hand;
          case (style::app_starting):
            return _app_starting;
          case (style::help):
          default:
            return _help;
        }
      }

    protected:
      template<typename ... _ArgTs>
      cursor(_ArgTs &&...oArgs) : shared_ptr(std::forward<_ArgTs>(oArgs)...) {}
    };
  }

