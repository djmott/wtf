#include <iostream>
#include "wtf.hpp"


struct main_form : wtf::form{
  main_form() : l(*this)
  {
    titlebar("eye candy example");
    l.move(5, 5, 10, 10);
    l.border_style(wtf::label::border_styles::etched);

/*
    OnResize.connect([this](const wtf::point::client_coords& p){
      std::cout << "[resize client width: " << p.x << " resize client  height: " << p.y << "]" << std::endl;
      std::cout << "[form width: " << this->width() << " form height: " << this->height() << "]" << std::endl;
      std::cout << "button top:" << b.top() << " left:" << b.left() << " width:" << b.width() << " height:" << b.height() << std::endl << std::endl;
//       std::cout << "[form left: " << this->left() << " form top: " << this->top() << "]" << std::endl << std::endl;
//       std::cout << "[client width: " << this->client_width() << " client height: " << this->client_height() << "]" << std::endl << std::endl;
    });

    OnMoved.connect([this](const wtf::point::client_coords& p){
      std::cout << "[move client left: " << p.x << " move client top: " << p.y << "]" << std::endl;
      std::cout << "[form left: " << this->left() << " form top: " << this->top() << "]" << std::endl;
      std::cout << "button top:" << b.top() << " left:" << b.left() << " width:" << b.width() << " height:" << b.height() << std::endl << std::endl;
//       std::cout << "[form width: " << this->width() << " form height: " << this->height() << "]" << std::endl << std::endl;
//       std::cout << "[client width: " << this->client_width() << " client height: " << this->client_height() << "]" << std::endl << std::endl;
    });
*/
  }
  wtf::label l;
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