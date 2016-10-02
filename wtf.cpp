#include <windows.h>

#include <iostream>




#include "wtf.hpp"

namespace theme = wtf::default_theme;

struct MyForm : theme::form{
  
  MyForm() : oButton(*this){
    oButton.move(10, 10, 150, 25);
    oButton.text(L"Hello World");
    oButton.OnClickEvent = [](){ std::cout << "Button clicked"; };
  }

  theme::button oButton;
};


int main(){
  MyForm oForm;
  oForm.show();
  wtf::message oMsg;
  return oMsg.pump();
}
