/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{

    template <typename _SuperT>
    struct has_move : _SuperT{
      
      enum class wm_size_flags{
        hide = SIZE_MAXHIDE,
        maximized = SIZE_MAXIMIZED,
        show = SIZE_MAXSHOW,
        minimized = SIZE_MINIMIZED,
        restored = SIZE_RESTORED,
      };

      void move(int x, int y, int width, int height, bool repaint = true){
        wtf::exception::throw_lasterr_if(::MoveWindow(*this, x, y, width, height, repaint ? TRUE : FALSE),
                                         [](BOOL b){ return !b; });
      }

      virtual int left() const{
        auto rc = rect<coord_frame::client>::get(*this);
        MapWindowPoints(*this, *_SuperT::_parent, reinterpret_cast<LPPOINT>(&rc), 2);
        return rc.left;
      }

      virtual int top() const{
        auto rc = rect<coord_frame::client>::get(*this);
        SetLastError(ERROR_INVALID_PARAMETER);
        MapWindowPoints(*this, *_SuperT::_parent, reinterpret_cast<LPPOINT>(&rc), 2);
        return rc.top;
      }


      virtual void left(int newval){
        wtf::exception::throw_lasterr_if(::SetWindowPos(*this, 0, newval, top(), 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOREPOSITION),
                                         [](BOOL b){ return !b; });
      }

      virtual void top(int newval){
        wtf::exception::throw_lasterr_if(::SetWindowPos(*this, 0, left(), newval, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOREPOSITION),
                                         [](BOOL b){ return !b; });
      }

    protected:

      explicit has_move(window * pParent) : _SuperT(pParent){}

    };
  }
}
