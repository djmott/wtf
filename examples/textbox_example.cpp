#include <iostream>
#include "wtf.hpp"


struct main_form : wtf::form{
  main_form() :
    oTextbox(*this)
  {
    titlebar("Textbox example");
    oTextbox.font().lfHeight = 25;
    oTextbox.move(5, 5, 100, 25);
    oTextbox.text("The quick brown fox jumped over the lazy dog.");
  }

  wtf::textbox oTextbox;
};


int main(){
  try{
    main_form oForm;
    return oForm.exec();
  }
  catch (const wtf::exception& ex){
    std::cerr << "An exception occurred." << std::endl << ex.what() << std::endl << std::endl;
    std::cerr << ex.file() << "(" << ex.line() << ")" << std::endl << std::endl;
    std::cerr << ex.code();
    return -1;
  }
}