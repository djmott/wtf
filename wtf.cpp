
#include <iostream>
#include "wtf.hpp"

struct MyForm : wtf::form{
  MyForm() : form(), oButton(*this) {}

  wtf::button oButton;
};

int main(){
  try{
    MyForm oForm;
    oForm.show();
    return oForm.run();
    return 0;
  }
  catch (const wtf::exception& ex){
    std::cerr << "A WTF exception occurred in " << ex.file() << "(" << ex.line() << ") : " << ex.code() << std::endl << ex.what() << std::endl;
  }
  return -1;
}