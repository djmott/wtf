/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#define DOXY_INHERIT_HAS_MOVE

namespace wtf{
  namespace policy{

    /** @class has_move
    @brief Behavior policy of a movable window
    @ingroup Policies
    */
    template <typename _super_t>
    struct has_move : _super_t{
      

      /** @brief Moves the window to the specified position and optionally forces a repaint
      @param[in] x,y new position of the window
      @param[in] width,height new size of the window
      @param[in] repaint indicates whether the window should be repainted
      */
      void move(int x, int y, int width, int height, bool repaint = true)  {
        wtf::exception::throw_lasterr_if(::MoveWindow(*this, x, y, width, height, repaint ? TRUE : FALSE),
                                         [](BOOL b)noexcept { return !b; });
      }

      //! @brief Gets the windows left position relative to its parent
      virtual int left() const{
        auto rc = rect<coord_frame::client>::get(*this);
        MapWindowPoints(*this, *_super_t::_parent, reinterpret_cast<LPPOINT>(&rc), 2);
        return rc.left;
      }

      //! @brief Gets the windows top position relative to its parent
      virtual int top() const{
        auto rc = rect<coord_frame::client>::get(*this);
        SetLastError(ERROR_INVALID_PARAMETER);
        MapWindowPoints(*this, *_super_t::_parent, reinterpret_cast<LPPOINT>(&rc), 2);
        return rc.top;
      }

      //! @brief Sets the windows left position
      //! @param[in] newval new left position of the window
      virtual void left(int newval)  {
        wtf::exception::throw_lasterr_if(::SetWindowPos(*this, 0, newval, top(), 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOREPOSITION),
                                         [](BOOL b)noexcept { return !b; });
      }

      //! @brief Sets the windows top position
      //! @param[in] newval new top position of the window
      virtual void top(int newval)  {
        wtf::exception::throw_lasterr_if(::SetWindowPos(*this, 0, left(), newval, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOREPOSITION),
                                         [](BOOL b)noexcept { return !b; });
      }

    protected:
      template <typename, template <typename> typename...> friend struct window_impl;

    };
  }
}
