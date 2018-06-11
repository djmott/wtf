#include "wtf/wtf.hpp"

using namespace wtf;

struct frmMain : form {

  frmMain() : form(), _tab(this), _button(this), _edit(this){

    _edit.OnCreated += [this](...) {
      _edit.text(_T("This is an edit box"));
      _tab.add(_T("author"), &_edit);
    };
    _button.OnCreated += [this](...){
      _button.text(_T("Clicky Clicky"));
      _tab.add(_T("clicky"), &_button);
    };
    OnSize += [this](...) {
      _tab.move(50, 50, width() - 70, height() - 70);
    };
  }

  controls::tab _tab;
  controls::button _button;
  controls::edit _edit;
};

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  return frmMain().run();
}
