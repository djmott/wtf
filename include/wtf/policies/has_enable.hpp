/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#define DOXY_INHERIT_HAS_ENABLE

namespace wtf{
  namespace policy{

    /** @class has_enable
    @brief Behavior policy of a window that can be enabled or disabled
    @ingroup Policies
    */
    template <typename _super_t>
    struct has_enable : _super_t{
  
      //! @brief Enables the window
      virtual void enable() noexcept { ::EnableWindow(*this, TRUE); }
      
      //! @brief Disables the window
      virtual void disable()  noexcept { ::EnableWindow(*this, FALSE); }
      
      /** @brief Indicates whether the window is enabled or disabled
      @returns true if enabled or false if disabled
      */
      virtual bool enabled() const  { return ((wtf::exception::throw_lasterr_if(::GetWindowLong(*this, GWL_STYLE), [](LONG l) noexcept {return !l; }) & WS_DISABLED)) ? false : true; }

      /** @brief Sets the enbled state of the window
      @param[in] newval set to true to enable or false to disable
      */
      virtual void enabled(bool newval){
        if (newval) enable();
        else disable();        
      }

    };
  }

}
