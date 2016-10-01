#include <windows.h>

#include <iostream>




#include "wtf.hpp"

using namespace wtf::default_theme;

struct MyForm : form{
  MyForm() : form(), _lb(*this){
    _lb.move(0, 0, 100, 100);
    for (int i = 0; i < 100; i++){
      _lb.items().push_back(std::to_wstring(i));
    }
  }
  listbox _lb;
};


int main(){
  try{
	  MyForm oForm;
    oForm.show();
    wtf::message oMsg;
    return oMsg.pump();
  }
  catch (const wtf::exception& ex){
    std::cerr << "A WTF exception occurred in " << ex.file() << "(" << ex.line() << ") : " << ex.code() << std::endl << ex.what() << std::endl;
  }
  catch (const std::exception& ex){
    std::cerr << "An STL exception occurred: " << ex.what() << std::endl;
  }
  return -1;
}
