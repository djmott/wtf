/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/

struct StatusBar : wtf::controls::statusbar {
  StatusBar() {
    OnCreated += [this](...) {
      parts().add(_T("Ready"));
      parts().add(_T("CAPS"));
      parts().add(_T("\t\tNUM       "), -1);
    };
  }
};