/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{

    /** has_focus
    * Provides set_focus and got_focus
    */
    template <typename _super_t>
    struct has_focus : _super_t{

      void set_focus() const  { wtf::exception::throw_lasterr_if(::SetFocus(*this), [](HWND h)noexcept { return !h; }); }

      bool got_focus() const{ return _super_t::_handle == ::GetFocus(); }

    };
  }
}
