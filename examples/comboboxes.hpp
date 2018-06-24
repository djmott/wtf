/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/

struct ComboBoxes : wtf::controls::label {
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
