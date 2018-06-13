/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/


#include "wtf/wtf.hpp"
using namespace wtf;

struct frmMain : form {

  frmMain() : form(){
//    add(_ctl);
    OnSize += [this](...){
      _ctl.move(10, 10, 200, 30);
    };
  }

  wtf::controls::date_time _ctl;
};

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  try {
    return frmMain().run();
  }
  catch (const wtf::exception& ex) {
    tstring sMsg = _T("An exception occurred at ") + to_tstring(ex.file()) + _T(":") + to_tstring(ex.line()) + _T("\n") +
      to_tstring(ex.code()) + _T("\n") + to_tstring(ex.what());
    wtf::message_box::exec(nullptr, sMsg);

  }
  return 0;
}
