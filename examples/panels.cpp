/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/

#include "wtf/wtf.hpp"

using namespace wtf;

struct frmMain : wtf::form {
  frmMain() : form(), _none(this), _raised(this), _lowered(this), _etched(this), _bumped(this), _double_raised(this), _double_lowered(this) {

    _none.border_style(border_styles::none);
    _raised.border_style(border_styles::raised);
    _lowered.border_style(border_styles::lowered);
    _etched.border_style(border_styles::etched);
    _bumped.border_style(border_styles::bumped);
    _double_lowered.border_style(border_styles::double_lowered);
    _double_raised.border_style(border_styles::double_raised);

    OnCreate += [this](...)    {
      auto position = [](wtf::panel& src, wtf::panel& dest, LPCTSTR caption){
        dest.text(caption);
        dest.move(5, src.top() + src.height() + 3, src.width(), src.height());
      };
      _none.move(5, 5, 100, 25);
      _none.text(_T("No border"));
      position(_none, _raised, "raised");
      position(_raised, _lowered, "lowered");
      position(_lowered, _etched, "etched");
      position(_etched, _bumped, "bumped");
      position(_bumped, _double_raised, "double raised");
      position(_double_raised, _double_lowered, "double lowered");
    };
  }

  panel _none, _raised, _lowered, _etched, _bumped, _double_raised, _double_lowered;
};

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  return frmMain().run();
}
