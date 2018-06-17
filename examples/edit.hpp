#include "wtf/wtf.hpp"

using namespace wtf;

struct frmMain : form {

  frmMain() : form(), _edit(this) {

    OnSize += [this](...) {
      _edit.move(10, 10, width() - 20, height() - 20);
    };
  }

  controls::edit _edit;
};

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  return frmMain().run();
}
