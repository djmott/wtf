#include "wtf/wtf.hpp"

using namespace wtf;

struct frmMain : form {
  frmMain() : form(), _button(this), _toggle(this){
    OnCreated += [this](...) {
      _button.text("Push Me");
      _button.move(10, 10, 70, 25);
      _toggle.text("Toggle Me");
      _toggle.move(10, 40, 70, 25);
    };
    _button.OnClick += [this](...) {
      auto oFont = _button.font();
      oFont.strikeout(!oFont.strikeout());
      _button.font(oFont);
    };
    _toggle.OnClick += [this](...) {
      if (_toggle.value()) {
        _toggle.fore_color(system_rgb<system_colors::button_text>());
      }else {
        _toggle.fore_color(static_rgb<255, 0, 0>());
      }
    };
  }

  button _button;
  toggle_button _toggle;
};

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  return frmMain().run();
}