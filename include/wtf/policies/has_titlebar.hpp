/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{

    /** has_titlebar
    * Adds titlebar text features to windows
    */
    template <typename _SuperT>
    struct has_titlebar : _SuperT{

      const tstring &titlebar() const{
        return _titlebar;
      }

      void titlebar(LPCTSTR newval){
        wtf::exception::throw_lasterr_if(::SetWindowText(*this, newval), [](BOOL b){ return !b; });
        _titlebar = newval;
      }

    protected:
      explicit has_titlebar(window * pParent) : _SuperT(pParent){}
    private:
      tstring _titlebar = _T("");
    };
  }
}
