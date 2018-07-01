/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/

#define WTF_USE_COMMON_CONTROLS 1
#define WTF_USE_VISUAL_STYLES 1
#define WTF_USE_RICHEDIT 1
#define WTF_DEBUG_MESSAGES 1

#include "wtf/wtf.hpp"
using namespace wtf;
using namespace wtf::controls;
#include "listviews.hpp"
#include "buttons.hpp"
#include "comboboxes.hpp"
#include "progressbars.hpp"
#include "trees.hpp"
#include "statusbars.hpp"
#include "menus.hpp"


struct frmMain : form {

  frmMain() {
    add(_tab);
    add(_status);
    add(_mnuMain);
    _tab.add(_listview);
    _tab.add(_richedit);
    _tab.add(_buttons);
    _tab.add(_comboboxes);
    _tab.add(_progressbars);
    _tab.add(_trees);
    OnCreated += [this](...) {
      _tab.recieves_focus(false);
      _tab.items().add(_T("Richedit"), _richedit);
      _tab.items().add(_T("Listview"), _listview);
      _tab.items().add(_T("Buttons"), _buttons);
      _tab.items().add(_T("Comboboxes"), _comboboxes);
      _tab.items().add(_T("Progress bars"), _progressbars);
      _tab.items().add(_T("Trees"), _trees);
    };
    OnSize += [this](...) {
      _tab.move(5, 5, width() - 10, height() - 30);
      _status.move(0, height() - 20, width(), 20);
    };
  }


  wtf::controls::tab _tab;
  wtf::controls::richedit<true> _richedit;
  ListViews _listview;
  Buttons _buttons;
  ComboBoxes _comboboxes;
  ProgressBars _progressbars;
  Trees _trees;
  mnuMain _mnuMain;
  StatusBar _status;
};


#if 1
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

#else
int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  try {

    struct frm : form {
      frm() {
        add(_ctl);
        OnCreated += [this](...) {
          move(400, 400, 400, 400);
          _ctl.items().add(_T("ABC"));
          _ctl.items().add(_T("DEF"));         
        };
        OnSize += [this](...) {
          _ctl.move(0, 0, width(), height());
        };
      }
      tab _ctl;
    };
    return frm().run();
  }
  catch (const wtf::exception& ex) {
    tstring sMsg = _T("An exception occurred at ") + to_tstring(ex.file()) + _T(":") + to_tstring(ex.line()) + _T("\n") +
      to_tstring(ex.code()) + _T("\n") + to_tstring(ex.what());
    wtf::message_box::exec(nullptr, sMsg);

  }
  return 0;
}
#endif