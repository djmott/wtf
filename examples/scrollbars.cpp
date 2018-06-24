/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#include "wtf/wtf.hpp"

using namespace wtf;

struct frmMain : form {
#if 0
  void ShowValues(int) {
    tstring sCaption = std::to_string(_hscroll.value()) + ":" + std::to_string(_vscroll.value());
    titlebar(sCaption);
  }

  frmMain() : form(), _hscroll(this), _vscroll(this) {
    _hscroll.orientation(orientations::horizontal);
    _hscroll.OnValueChanged.connect<frmMain, &frmMain::ShowValues>(this);
    _vscroll.orientation(orientations::vertical);
    _vscroll.OnValueChanged.connect<frmMain, &frmMain::ShowValues>(this);

    OnSize += [this](...) {
      _hscroll.move(5, 5, this->width() - 20, 25);
      _vscroll.move(5, 25, 25, this->height() - 40);
    };
  }
  scroll_bar _hscroll, _vscroll;
#endif
};

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  return frmMain().run();
}
