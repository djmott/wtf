/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{

    template <typename _super_t>
    struct has_move : _super_t{
      
      enum class wm_size_flags{
        hide = SIZE_MAXHIDE,
        maximized = SIZE_MAXIMIZED,
        show = SIZE_MAXSHOW,
        minimized = SIZE_MINIMIZED,
        restored = SIZE_RESTORED,
      };

      void move(int x, int y, int width, int height, bool repaint = true)  {
        wtf::exception::throw_lasterr_if(::MoveWindow(*this, x, y, width, height, repaint ? TRUE : FALSE),
                                         [](BOOL b)noexcept { return !b; });
      }

      virtual int left() const{
        auto rc = rect<coord_frame::client>::get(*this);
        MapWindowPoints(*this, *_super_t::_parent, reinterpret_cast<LPPOINT>(&rc), 2);
        return rc.left;
      }

      virtual int top() const{
        auto rc = rect<coord_frame::client>::get(*this);
        SetLastError(ERROR_INVALID_PARAMETER);
        MapWindowPoints(*this, *_super_t::_parent, reinterpret_cast<LPPOINT>(&rc), 2);
        return rc.top;
      }


      virtual void left(int newval)  {
        wtf::exception::throw_lasterr_if(::SetWindowPos(*this, 0, newval, top(), 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOREPOSITION),
                                         [](BOOL b)noexcept { return !b; });
      }

      virtual void top(int newval)  {
        wtf::exception::throw_lasterr_if(::SetWindowPos(*this, 0, left(), newval, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOREPOSITION),
                                         [](BOOL b)noexcept { return !b; });
      }

    protected:
      template <typename, template <typename> typename...> friend struct window_impl;

      explicit has_move(window * pParent)  : _super_t(pParent){}

    };
  }
}
