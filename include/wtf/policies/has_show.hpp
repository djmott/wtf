/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{

    /** has_show
    * Adds show() and hide() members
    */
    template <typename _SuperT>
    struct has_show :  _SuperT{

      virtual void show(){ ::ShowWindow(*this, SW_SHOW); }

      virtual void hide(){ ::ShowWindow(*this, SW_HIDE); }

    protected:

      explicit has_show(window * pParent) : _SuperT(pParent){}

    };

  }
}
