#include <iostream>
#include "wtf.hpp"


struct main_form : wtf::form{
  main_form() : l1(*this), l2(*this), l3(*this)
  {
    titlebar("eye candy example");
    l1.move(100, 100, 100, 100);
    l1.border_style(wtf::label::border_styles::etched);
    l2.move(150, 150, 100, 100);
    l2.border_style(wtf::label::border_styles::bumped);
    l3.move(120, 120, 100, 100);
    l3.border_style(wtf::label::border_styles::double_raised);

    l1.OnClick += [this](){ l1.zorder(wtf::label::zorders::top); };
    l2.OnClick += [this](){ l2.zorder(wtf::label::zorders::top); };
    l3.OnClick += [this](){ l3.zorder(wtf::label::zorders::top); };

//     l1.zorder(l2);

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
  wtf::label l1, l2, l3;
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