#include <iostream>
#include "wtf.hpp"

using namespace wtf::default_theme;

struct MyForm : form{
  MyForm() : oText(*this){
    oText.move(10, 10, 100, 25);
    oText.text(_T("FNORD!!!"));
  }

  textbox oText;
};

int main(){
  try{
    MyForm oForm;
    oForm.show();
    wtf::message oMsg;
    return oMsg.pump();
  }
  catch (const wtf::exception& ex){
    std::cout << ex.what() << std::endl;
  }
  return -1;
}