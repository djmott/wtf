#include "wtf/wtf.hpp"

using namespace wtf;

struct frmMain : form {
  
  frmMain() : form(), _button(this), _checkbox(this), _tristate(this),
    _option_a(this), _option_b(this), _option_c(this),
    _option_x(this), _option_y(this), _option_z(this)
  {

    OnCreated += [this](...) {
      int _top = 10;
      auto top = [&_top]() { auto ret = _top; _top += 20; return ret; };
      _button.text("Click Me");
      _button.move(10, top(), 70, 25);
      _checkbox.text("Check me");
      _checkbox.move(10, top(), 100, 25);
      _tristate.text("Tristate me");
      _tristate.move(10, top(), 100, 25);
      _option_a.text("A");
      _option_a.move(10, top(), 100, 25);
      _option_b.text("B");
      _option_b.move(10, top(), 100, 25);
      _option_c.text("C");
      _option_c.move(10, top(), 100, 25);
      _option_x.text("X");
      _option_x.move(10, top(), 100, 25);
      _option_y.text("Y");
      _option_y.move(10, top(), 100, 25);
      _option_z.text("Z");
      _option_z.move(10, top(), 100, 25);
    };

  }

  controls::button _button;
  controls::checkbox _checkbox;
  controls::tristate _tristate;
  controls::radio_group _option_a;
  controls::radio_button _option_b;
  controls::radio_button _option_c;
  controls::radio_group _option_x;
  controls::radio_button _option_y;
  controls::radio_button _option_z;
};

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  return frmMain().run();
}
