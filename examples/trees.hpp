/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#include "wtf/wtf.hpp"

using namespace wtf;

struct Trees : wtf::controls::tree {

  Trees()  {
    OnCreated += [this](...) {
      auto oColors = items().add(_T("Colors"));
      oColors->items().add(_T("Red"));
      oColors->items().add(_T("Orange"));
      oColors->items().add(_T("Yellow"));
      oColors->items().add(_T("Green"));
      oColors->items().add(_T("Blue"));
      oColors->items().add(_T("Purple"));
      oColors->items().add(_T("White"));
      auto oNumbers = items().add(_T("Numbers"));
      oNumbers->items().add(_T("1"));
      oNumbers->items().add(_T("2"));
      oNumbers->items().add(_T("3"));
      oNumbers->items().add(_T("4"));
      oNumbers->items().add(_T("5"));
    };
    
  }
 
};

