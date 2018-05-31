/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{

    /** has_enable
    * Add enable/disable/enabled method
    */
    template <typename _SuperT>
    struct has_enable : _SuperT{
  
      virtual void enable()  {
        wtf::exception::throw_lasterr_if(::EnableWindow(*this, TRUE), [](BOOL b)noexcept { return !b; });
      }
      
      virtual void disable()  {
        wtf::exception::throw_lasterr_if(::EnableWindow(*this, FALSE), [](BOOL b)noexcept { return !b; });
      }
      
      virtual bool enabled() const  {
        return ((wtf::exception::throw_lasterr_if(::GetWindowLong(*this, GWL_STYLE), [](LONG l)noexcept {return !l; }) & WS_DISABLED)) ? false : true;
      }

      virtual void enabled(bool newval){
        if (newval) enable();
        else disable();        
      }

    protected:

      explicit has_enable(window * pParent) noexcept : _SuperT(pParent){}

    };
  }

}
