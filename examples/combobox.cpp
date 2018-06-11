#include "wtf/wtf.hpp"

using namespace wtf;

struct frmMain : form {

  frmMain() : form(), _simple(this), _edit(this), _static(this) {

    OnCreated += [this](...) {
      _simple.move(10, 10, 100, 100);
      _edit.move(120, 10, 100, 100);
      _static.move(240, 10, 100, 100);
      for (char x = 'z'; x >= 'a'; --x) {
        tstring str = "Item ";
        str += x;
        _simple.add_item(str);
        _edit.add_item(str);
        _static.add_item(str);
      }
      _static.OnSelChange += [this](...) {
        if (_static.selected_item()) this->titlebar(_static.selected_item()->text());
      };
    };

  }

  controls::combobox<true, controls::combobox_styles::simple> _simple;
  controls::combobox<false, controls::combobox_styles::drop_down> _edit;
  controls::combobox<true, controls::combobox_styles::drop_down_list> _static;
};

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  return frmMain().run();
}
