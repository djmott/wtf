#include "wtf/wtf.hpp"

using namespace wtf;

struct frmMain : form {

  frmMain() : form(), _tab(this), _button(&_tab), _edit(&_tab){

    _tab.OnCreated += [this](...) {
      _tab.add_item(_T("author"), &_edit);
      _tab.add_item(_T("clicky"), &_button);
      _edit.text(_T("This is an edit box"));
      _button.text(_T("Clicky Clicky"));
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
