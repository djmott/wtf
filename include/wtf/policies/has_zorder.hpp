/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  enum class zorders{
    bottom = 1,
    no_top_most = -2,
    top = 0,
    top_most = -1,
  };

  namespace policy{

    template <typename _super_t>
    struct has_zorder : _super_t{

      void zorder(zorders pos)  {
        wtf::exception::throw_lasterr_if(::SetWindowPos(*this, reinterpret_cast<HWND>(pos), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOCOPYBITS), [](BOOL b)noexcept { return !b; });
      }
      void zorder(HWND insert_after)  {
        wtf::exception::throw_lasterr_if(::SetWindowPos(*this, insert_after, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOCOPYBITS), [](BOOL b)noexcept { return !b; });
      }
    protected:
      explicit has_zorder(window * pParent)  : _super_t(pParent){}

    };
  }
}
