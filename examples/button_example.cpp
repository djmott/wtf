#include <iostream>
#include "wtf.hpp"


struct main_form : wtf::form{


  main_form() : wtf::form(), oPush(*this), oPush2(*this), oToggle(*this){
    titlebar("Buttons Example");
    oPush.text("&Click Me");
    oToggle.text("Toggle Me");
    oPush2.border_style(wtf::push_button::border_styles::double_raised);
    OnResize.connect([this](const wtf::point::client_coords& p){
      oPush.move(5, 5, 75, 35);
      position(oPush, oPush2);
      position(oPush2, oToggle);
    });
  }

private:


  template <typename _T1, typename _T2>
  void position(_T1& src, _T2& dest){
    dest.move(src.left() + src.width() + 5, src.top(), src.width(), src.height());
  }

  wtf::push_button oPush;
  wtf::push_button oPush2;
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
