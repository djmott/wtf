#include "wtf/wtf.hpp"

using namespace wtf;

struct Trees : wtf::controls::label {

  Trees() : wtf::controls::label(), _tree() {
    add(_tree);
    OnSize += [this](...) {
      _tree.move(10, 10, width() - 20, height() - 20);
    };
    OnCreated += [this](...) {
      auto oColors = _tree.items().add(_T("Colors"));
      oColors->items().add(_T("Red"));
      oColors->items().add(_T("Orange"));
      oColors->items().add(_T("Yellow"));
      oColors->items().add(_T("Green"));
      oColors->items().add(_T("Blue"));
      oColors->items().add(_T("Purple"));
      oColors->items().add(_T("White"));
      auto oNumbers = _tree.items().add(_T("Numbers"));
      oNumbers->items().add(_T("1"));
      oNumbers->items().add(_T("2"));
      oNumbers->items().add(_T("3"));
      oNumbers->items().add(_T("4"));
      oNumbers->items().add(_T("5"));
    };
    
  }
 
  wtf::controls::tree _tree;
};

