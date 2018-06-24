/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/

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
      _option_z.enabled(false);
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
