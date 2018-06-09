#include "wtf/wtf.hpp"

using namespace wtf;

struct frmMain : form {

  frmMain() : form(), _simple(this), _edit(this), _static(this) {

    OnCreated += [this](...) {
      _simple.text("Push Me");
      _simple.move(10, 10, 70, 25);
    };

  }

  controls::sorted_simple_combobox _simple;
  controls::sorted_edit_combobox _edit;
  controls::sorted_static_combobox _static;
};

#if defined(__WTF_DEBUG_MESSAGES__)
int main() {
#else
int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#endif
  return frmMain().run();
}
