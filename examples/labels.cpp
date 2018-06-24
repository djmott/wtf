/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/

#include "wtf/wtf.hpp"

using namespace wtf;

struct frmMain : form {

  frmMain() : form(), _label1(this) {

    OnCreate += [this](...) {
      _label1.text(_T("I'm a label"));
      _label1.move(10, 10, 100, 25);
    };
  }
  wtf::controls::label _label1;


};

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  return frmMain().run();
}
