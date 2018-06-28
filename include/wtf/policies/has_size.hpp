/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#define DOXY_INHERIT_HAS_SIZE

namespace wtf{
  namespace policy{

    /** @class has_size
    @brief Behavior policy of a window that has dimensions
    @ingroup Policies
    */
    template <typename _super_t>
    struct has_size : _super_t{
      //! @brief Gets width of the window
      virtual int width() const{
        auto r = rect<coord_frame::screen>::get(*this);
        return r.right - r.left;
      }

      //! @brief Gets the height of the window
      virtual int height() const{
        auto r = rect<coord_frame::screen>::get(*this);
        return r.bottom - r.top;
      }

      //! @brief Sets the width of the window
      virtual void width(int newval)  {
        wtf::exception::throw_lasterr_if(::SetWindowPos(*this, 0, 0, 0, newval, height(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOREPOSITION),
                                         [](BOOL b)noexcept { return !b; });
      }

      //! @brief Sets the height of the window
      virtual void height(int newval)  {
        wtf::exception::throw_lasterr_if(::SetWindowPos(*this, 0, 0, 0, width(), newval, SWP_NOMOVE | SWP_NOZORDER | SWP_NOREPOSITION),
                                         [](BOOL b)noexcept { return !b; });
      }

    };
  }
}
