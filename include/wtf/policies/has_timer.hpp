/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{

    /** has_timer
    * Adds timer creation and produces timer events
    */
    template <typename _super_t>
    struct has_timer : _super_t{

      UINT_PTR set_timer(UINT elapse)  {
        _next_timer_id++;
        return wtf::exception::throw_lasterr_if(::SetTimer(*this, _next_timer_id, elapse, nullptr),
                                                [](UINT_PTR x)noexcept { return !x; });
      }

      void set_timer(UINT elapse, UINT_PTR timer_id)  {
        wtf::exception::throw_lasterr_if(::SetTimer(*this, timer_id, elapse, nullptr),
                                         [](UINT_PTR x)noexcept { return !x; });
      }

      void kill_timer(UINT_PTR timer_id)  {
        wtf::exception::throw_lasterr_if(::KillTimer(*this, timer_id), [](BOOL x)noexcept { return !x; });
      }

    protected:

      explicit has_timer(window * pParent) noexcept : _super_t(pParent){}
    private:
      UINT_PTR _next_timer_id = 1;
    };
  }

  namespace _{
    template <> struct policy_traits<policy::has_timer>{
      using requires = policy_list<policy::wm_timer>;
    };
  }

}