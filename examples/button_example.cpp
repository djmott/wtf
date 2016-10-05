#include <iostream>
#include "wtf.hpp"


struct main_form : wtf::form{
  main_form() : wtf::form(), oPush(*this), oToggle(*this){
    titlebar("Buttons Example");
    oPush.move(5,5,75,35);
    oToggle.move(5, 100, 75, 30);
    oPush.text("&Click Me");
    oToggle.text("Toggle Me");
  }

  wtf::push_button oPush;
  wtf::toggle_button oToggle;

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
