#include <iostream>
#include "wtf.hpp"


struct main_form : wtf::form{
  main_form() :
    wtf::form(),
    oListbox(*this)
  {
    titlebar("Label example");
    oListbox.font().height(25);
    oListbox.move(5, 5, 100, 200);
    oListbox.lock_update();
    for (int i = 0; i < 100; i++){
      auto sTemp = std::to_string(i);
      wtf::tstring sItem;
      std::copy(sTemp.begin(), sTemp.end(), std::back_inserter(sItem));
      oListbox.add_item(sItem);
    }
    oListbox.unlock_update();
  }

  wtf::listbox oListbox;
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