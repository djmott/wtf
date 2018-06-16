/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {

    namespace policy {
      /** @class has_enable
      Specifies an enabled or disabled behavior
      @ingroup Policies
      */

      template <typename _super_t>
      struct has_enable: _super_t {

        /**
        * @fn bool enabled()
        * Gets the enabled state
        * @return true if enabled or false if disabled
        */
        bool enabled() const { return !_super_t::get_style<WS_DISABLED>(); }
        /**
         * @fn void enabled(bool newval)
         * Enables or disables the widget
         * @param[in] newval true to enable or false to disable
         */
        void enabled(bool newval) { _super_t::set_style<WS_DISABLED>(!newval); }

      protected:
        template <typename, template <typename> typename...> friend struct window_impl;

      };
    }
  }
}