#include <iostream>
#include "wtf.hpp"

struct floating_child_form : wtf::form_base<floating_child_form, WS_EX_TOOLWINDOW, WS_VISIBLE | WS_BORDER | WS_CAPTION>{
  floating_child_form(HWND parent) : form_base(parent){
    titlebar("A floating child form");
    move(100, 100, 300, 300);
  }
};

struct embedded_child_form : wtf::form_base<embedded_child_form, WS_EX_TOOLWINDOW, WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CAPTION|WS_CLIPSIBLINGS>{
  embedded_child_form(HWND parent) : form_base(parent){
    titlebar("An embedded child form");
    move(20, 20, 200, 500);
  }
};


struct main_form : wtf::form{
  main_form() : wtf::form(), _embedded_form(*this), _floating_form(*this){
    titlebar("A standard form");
    _embedded_form.show();
    _floating_form.show();
  }

  embedded_child_form _embedded_form;
  floating_child_form _floating_form;
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