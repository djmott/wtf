#include "wtf/wtf.hpp"
using namespace wtf;

struct frmMain : form {
#if 0
  frmMain() : form(), _hprogressbar(this), _vprogressbar(this){
    OnCreate += [this](...) {
      _hprogressbar.orientation(orientations::horizontal);
      _hprogressbar.value(50);
      _vprogressbar.orientation(orientations::vertical);
      _vprogressbar.value(50);
    };
    OnSize += [this](...) {
      _hprogressbar.move(10, 10, this->width() - 20, 25);
      _vprogressbar.move(10, 40, 20, this->height() - 60);
    };
  }
  progress_bar _hprogressbar, _vprogressbar;
#endif
};

#if defined(__WTF_DEBUG_MESSAGES__)
int main() {
#else
int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#endif
  return frmMain().run();
}
