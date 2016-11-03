/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{

    /** @class has_size
    Adds members to reposition/resize
    @addtogroup Policy
    */
    template <typename _SuperT>
    struct has_size : _SuperT{

      virtual int width() const{
        auto r = rect<coord_frame::client>::get(*this);
        return r.right - r.left;
      }

      virtual int height() const{
        auto r = rect<coord_frame::client>::get(*this);
        return r.bottom - r.top;
      }


      virtual void width(int newval){
        wtf::exception::throw_lasterr_if(::SetWindowPos(*this, 0, 0, 0, newval, height(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOREPOSITION),
                                         [](BOOL b){ return !b; });
      }

      virtual void height(int newval){
        wtf::exception::throw_lasterr_if(::SetWindowPos(*this, 0, 0, 0, width(), newval, SWP_NOMOVE | SWP_NOZORDER | SWP_NOREPOSITION),
                                         [](BOOL b){ return !b; });
      }

    protected:

      explicit has_size(iwindow * pParent) : _SuperT(pParent){}
    };
  }
}
