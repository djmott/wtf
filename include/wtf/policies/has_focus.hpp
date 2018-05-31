/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{

    /** has_focus
    * Provides set_focus and got_focus
    */
    template <typename _SuperT>
    struct has_focus : _SuperT{

      void set_focus() const  { wtf::exception::throw_lasterr_if(::SetFocus(*this), [](HWND h)noexcept { return !h; }); }

      bool got_focus() const{ return _SuperT::_handle == ::GetFocus(); }

    protected:

      explicit has_focus(window * pParent) noexcept : _SuperT(pParent){}

    };
  }
}
