#include <iostream>
#include "wtf.hpp"

using namespace wtf;

struct main_form : form{
  main_form() :
    oSplitter(*this)
  {
    titlebar("Splitter example");
    OnResize += [this](const point::client_coords& p){
      oSplitter.move(5, 5, p.x-10, p.y-10);
    };
    oSplitter.set_split_position(25);
  }

  split_container oSplitter;
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