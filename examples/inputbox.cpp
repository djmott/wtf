#include "wtf/wtf.hpp"

using namespace wtf;

struct frmMain : form {
#if 0
	frmMain() : form(), _inputbox(this) {
		OnCreate += [this](...) {
			_inputbox.text("Enter text here");
		};
		OnSize += [this](...) {
			_inputbox.move(15, 15, this->width()-30, 20);
		};

	}

	inputbox _inputbox;
#endif
};

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  return frmMain().run();
}
