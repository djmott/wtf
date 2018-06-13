/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{

    /** has_titlebar
    * Adds titlebar text features to windows
    */
    template <typename _super_t>
    struct has_titlebar : _super_t{

      const tstring &titlebar() const{
        return _titlebar;
      }

      void titlebar(const tstring& newval)  {
        wtf::exception::throw_lasterr_if(::SetWindowText(*this, newval.c_str()), [](BOOL b){ return !b; });
        _titlebar = newval;
      }

      has_titlebar() : _super_t(){}
    private:
      tstring _titlebar = _T("");
    };
  }
}
