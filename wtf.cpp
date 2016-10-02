#include <windows.h>

#include <iostream>




#include "wtf.hpp"

namespace theme = wtf::default_theme;

struct MyForm : theme::form{
  
  MyForm() : oList(*this){
    oList.move(10, 10, 500, 500);
    for (int i = 0; i < 100; i++){
      oList.add_item(std::to_wstring(i));
    }
  }

  theme::listbox oList;
};


int main(){
  MyForm oForm;
  oForm.show();
  wtf::message oMsg;
  return oMsg.pump();
}
