#include "wtf/wtf.hpp"

using namespace wtf;

struct frmMain : form {
#if 0
  frmMain() : form(), _textbox(this) {
    OnCreate += [this](...) {
      _textbox.text("Enter text here");
    };
    OnSize += [this](...) {
		_textbox.move(15, 15, this->width() - 30, 20);
    };

  }

  textbox _textbox;
#endif
};

#if defined(__WTF_DEBUG_MESSAGES__)
int main() {
#else
int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#endif
  return frmMain().run();
}
