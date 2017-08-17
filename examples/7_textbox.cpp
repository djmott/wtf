#include "wtf/wtf.hpp"

using namespace wtf;

struct frmMain : form {
  frmMain() : form(), _textbox(this) {
    OnCreate += [this](...) {
      _textbox.text("Enter text here");
    };
    OnSize += [this](...) {
      _textbox.move(0, 0, this->width(), 15);
    };

  }

  textbox _textbox;

};

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  return frmMain().run();
  return 0;
}