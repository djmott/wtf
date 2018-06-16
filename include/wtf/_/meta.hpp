/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace _ {
    template <bool _compare, typename _value_t, _value_t _true_v, _value_t _false_v> struct conditional_v {
      static constexpr _value_t value = _true_v;
    };

    template <typename _value_t, _value_t _true_v, _value_t _false_v> struct conditional_v<false,_value_t, _true_v, _false_v> {
      static constexpr _value_t value = _false_v;
    };


  }
}