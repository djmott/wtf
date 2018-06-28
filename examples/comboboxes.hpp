/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/

struct ComboBoxes : wtf::controls::label {
  ComboBoxes() : wtf::controls::label() {
    add(_simple);
    add(_dropdown);
    add(_dropdown_list);

    OnCreated += [this](...) {
      _simple.move(10, 10, 100, 100);
      _dropdown.move(120, 10, 100, 100);
      _dropdown_list.move(240, 10, 100, 100);
      for (TCHAR x = _T('z'); x >= _T('a'); --x) {
        tstring str = _T("Item ");
        str += x;
        _simple.add_item(str);
        _dropdown.add_item(str);
        _dropdown_list.add_item(str);
      }
    };
  }
  controls::simple_combobox _simple;
  controls::dropdown_combobox _dropdown;
  controls::dropdown_list_combobox _dropdown_list;
};
