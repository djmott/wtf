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

#if defined(__WTF_DEBUG_MESSAGES__)
int main() {
#else
int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#endif
  return frmMain().run();
}
