/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#include "wtf/wtf.hpp"

using namespace wtf;

struct frmMain : form {
#if 0
  frmMain() : form(), _textbox(this) {
    OnCreate += [this](...) {
      _textbox.text("Enter text here");
    };
    OnSize += [this](...) {
    _textbox.move(15, 15, this->width() - 30, 20);
    };

  }

  textbox _textbox;
#endif
};

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  return frmMain().run();
}
