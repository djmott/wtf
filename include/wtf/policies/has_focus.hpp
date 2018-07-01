/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#define DOXY_INHERIT_HAS_FOCUS

namespace wtf{
  namespace policy{

    /** @class has_focus
    @brief Provides set_focus and got_focus
    @ingroup Policies
    */
    template <typename _super_t>
    struct has_focus : _super_t{

      //! @brief Sets input focus on the window
      virtual void set_focus() const  { wtf::exception::throw_lasterr_if(::SetFocus(*this), [](HWND h)noexcept { return !h; }); }

      //! @brief Determines if the window has input focus
      virtual bool got_focus() const{ return _super_t::_handle == ::GetFocus(); }

    };
  }
}
