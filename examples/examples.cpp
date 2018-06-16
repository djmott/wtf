/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/

#include "wtf/wtf.hpp"
using namespace wtf;

struct Buttons : wtf::controls::label {

  Buttons() : label() {
    OnCreated += [this](...) {
      add(_button);
      add(_checkbox);
      add(_tristate);
      add(_option_a);
      add(_option_b);
      add(_option_c);
      add(_option_x);
      add(_option_y);
      add(_option_z);

      int _top = 10;
      auto top = [&_top]() { auto ret = _top; _top += 20; return ret; };
      _button.text(_T("Click Me"));
      _button.move(10, top(), 70, 25);
      _checkbox.text(_T("Check me"));
      _checkbox.move(10, top(), 100, 25);
      _tristate.text(_T("Tristate me"));
      _tristate.move(10, top(), 100, 25);
      _option_a.text(_T("A"));
      _option_a.move(10, top(), 100, 25);
      _option_b.text(_T("B"));
      _option_b.move(10, top(), 100, 25);
      _option_c.text(_T("C"));
      _option_c.move(10, top(), 100, 25);
      _option_x.text(_T("X"));
      _option_x.move(10, top(), 100, 25);
      _option_y.text(_T("Y"));
      _option_y.move(10, top(), 100, 25);
      _option_z.text(_T("Z"));
      _option_z.move(10, top(), 100, 25);
    };

  }

  wtf::controls::button _button;
  wtf::controls::checkbox _checkbox;
  wtf::controls::tristate _tristate;
  wtf::controls::radio_group _option_a;
  wtf::controls::radio_button _option_b;
  wtf::controls::radio_button _option_c;
  wtf::controls::radio_group _option_x;
  wtf::controls::radio_button _option_y;
  wtf::controls::radio_button _option_z;
};


struct ComboBoxes: wtf::controls::label{
  ComboBoxes() : wtf::controls::label() {

    OnCreated += [this](...) {
      add(_simple);
      add(_edit);
      add(_static);
      _simple.move(10, 10, 100, 100);
      _edit.move(120, 10, 100, 100);
      _static.move(240, 10, 100, 100);
      for (TCHAR x = _T('z'); x >= _T('a'); --x) {
        tstring str = _T("Item ");
        str += x;
        _simple.add_item(str);
        _edit.add_item(str);
        _static.add_item(str);
      }
    };
  }
  controls::combobox<true, controls::combobox_styles::simple> _simple;
  controls::combobox<false, controls::combobox_styles::drop_down> _edit;
  controls::combobox<true, controls::combobox_styles::drop_down_list> _static;
};

struct frmMain : form {

  frmMain() : form(), _tab() {
    add(_tab);
    _tab.add(_buttons);
    _tab.add(_comboboxes);

    OnCreated += [this](...) {
//      _tab.add(_buttons);
      _tab.items().add(_T("Buttons"), _buttons);
      _tab.items().add(_T("Comboboxes"), _comboboxes);
    };
    OnSize += [this](...) {
      _tab.move(5, 5, width() - 10, height() - 10);
    };
  }

  wtf::controls::tab _tab;
  Buttons _buttons;
  ComboBoxes _comboboxes;
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
