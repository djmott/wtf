#include <iostream>
#include "wtf.hpp"


struct main_form : wtf::form{
  main_form() :
    oTabs(*this)
  {
    titlebar("Tab container example");
    OnResize.connect([this](const wtf::point::client_coords& p){ 
      oTabs.move(5, 5, p.x-10, p.y-10); 
    });
    for (int i = 0; i < 10; i++){
      wtf::tstring sTemp = _T("ERER");
      oTabs.add_page(sTemp);
    }
  }

  wtf::tab_container oTabs;
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