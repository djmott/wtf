#include "wtf/wtf.hpp"

using namespace wtf;

struct frmMain : form {
#if 0
  frmMain() : form(), _tree(this){

    _tree.OnItemSelected += [this](tree::item::pointer oItem) {
      titlebar(oItem->text());
    };

    OnSize += [this](...) {_tree.move(0, 0, width(), height()); };

    OnCreate += [this](...) {
      auto oHot = _tree.add_item("Hot");
      auto oRedish = oHot->add_item("Redish");
      oRedish->add_item("Red");
      oRedish->add_item("Orange");
      oRedish->add_item("Yellow");
      auto oCold = _tree.add_item("Cold");
      auto oBlueish = oCold->add_item("Blueish");
      oBlueish->add_item("Blue");
      oBlueish->add_item("Violet");
      oBlueish->add_item("Purple");
      auto oBlackish = oCold->add_item("Blackish");
      oBlackish->add_item("Black");
      oBlackish->add_item("Brown");
    };
  }
  tree _tree;
#endif
};

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  return frmMain().run();
}
