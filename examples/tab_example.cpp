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


    auto & p1 = oTabs.add_page("page1");
    _b1 = std::make_unique<wtf::push_button>(p1);
    _b1->text("FNORD!");
    _b1->move(5, 5, 100, 20);

    auto & p2 = oTabs.add_page("page2");
    _b2 = std::make_unique<wtf::push_button>(p2);
    _b2->text("SNAFOO");
    _b2->move(5, 5, 100, 20);

    auto & p3 = oTabs.add_page("page3");
    _b3 = std::make_unique<wtf::push_button>(p3);
    _b3->text("Fizzbang");
    _b3->move(5, 5, 100, 20);


  }
  std::unique_ptr<wtf::push_button> _b1, _b2, _b3;

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
    assert(false);
    return -1;
  }
}