/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{

    /** has_show
    * Adds show/hide/visible members
    */
    template <typename _SuperT>
    struct has_show :  _SuperT{

      virtual void show()  { ::ShowWindow(*this, SW_SHOW); }

      virtual void hide()  { ::ShowWindow(*this, SW_HIDE); }

      virtual bool visible() const  {
        return (wtf::exception::throw_lasterr_if(
          ::GetWindowLong(*this, GWL_STYLE), [](LONG l)noexcept { return !l; })
          & WS_VISIBLE) ? true : false; 
      }

      virtual void visible(bool newval)  {
        if (newval) show();
        else hide();
      }

    protected:

      explicit has_show(window * pParent) noexcept : _SuperT(pParent){}

    };

  }
}
