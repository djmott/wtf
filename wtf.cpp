#include <iostream>
#include "wtf.hpp"

using namespace wtf::default_theme;

struct MyForm : form{
  

  MyForm() : oList(*this)
  {
    for (int i = 0; i < 1000; i++){
      oList.add_item(std::to_wstring(i));
    }
    ResizedEvent.connect([this](wm_size_flags, int width, int height){
      oList.move(0, 0, width, height);
    });
  }

  listbox oList;

};


int main(){
  MyForm oForm;
  oForm.show();
  wtf::message oMsg;
  return oMsg.pump();
}
