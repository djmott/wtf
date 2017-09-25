#include "wtf/wtf.hpp"

using namespace wtf;

struct frmMain : form {
  frmMain() : form(), _textbox(this) {
    OnCreate += [this](...) {
      _textbox.text("Enter text here");
    };
    OnSize += [this](...) {
		_textbox.move(15, 15, this->width() - 30, 20);
    };

  }

  textbox _textbox;

};

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  return frmMain().run();
}