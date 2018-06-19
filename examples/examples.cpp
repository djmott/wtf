/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/

#define WTF_USE_COMMON_CONTROLS 1
#define WTF_USE_VISUAL_STYLES 0
#define WTF_USE_RICHEDIT 1

#include "wtf/wtf.hpp"
using namespace wtf;
#include "listviews.hpp"
#include "buttons.hpp"
#include "comboboxes.hpp"
#include "progressbars.hpp"
#include "trees.hpp"

struct frmMain : form {

  frmMain() {
    add(_tab);
    _tab.add(_listview);
    _tab.add(_richedit);
    _tab.add(_buttons);
    _tab.add(_comboboxes);
    _tab.add(_progressbars);
    _tab.add(_trees);
    _tab.add(_avi);
    OnCreated += [this](...) {
      _tab.items().add(_T("Richedit"), _richedit);
      _tab.items().add(_T("Listview"), _listview);
      _tab.items().add(_T("Buttons"), _buttons);
      _tab.items().add(_T("Comboboxes"), _comboboxes);
      _tab.items().add(_T("Progress bars"), _progressbars);
      _tab.items().add(_T("Trees"), _trees);
      _tab.items().add(_T("AVI"), _avi);
    };
    OnSize += [this](...) {
      _tab.move(5, 5, width() - 10, height() - 10);
    };
  }


  wtf::controls::tab _tab;
  wtf::controls::richedit<true> _richedit;
  ListViews _listview;
  Buttons _buttons;
  ComboBoxes _comboboxes;
  ProgressBars _progressbars;
  Trees _trees;
  wtf::controls::avi_player _avi;
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
