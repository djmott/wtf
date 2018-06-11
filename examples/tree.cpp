#include "wtf/wtf.hpp"

using namespace wtf;

struct frmMain : form {

  frmMain() : form(), _tree(this) {

    OnSize += [this](...) {
      _tree.move(10, 10, width() - 20, height() - 20);
    };
    OnCreate += [this](...) {
      auto oColors = _tree.add_item(_T("Colors"));
      _tree.add_item(oColors, "Red");
      _tree.add_item(oColors, "Orange");
      _tree.add_item(oColors, "Yellow");
      _tree.add_item(oColors, "Green");
      _tree.add_item(oColors, "Blue");
      _tree.add_item(oColors, "Purple");
      _tree.add_item(oColors, "White");
      auto oNumbers = _tree.add_item(_T("Numbers"));
      _tree.add_item(oNumbers, "1");
      _tree.add_item(oNumbers, "2");
      _tree.add_item(oNumbers, "3");
      _tree.add_item(oNumbers, "4");
      _tree.add_item(oNumbers, "5");
    };
    
  }
#if 0
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
#endif
 
  wtf::controls::tree _tree;


};

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  return frmMain().run();
}
