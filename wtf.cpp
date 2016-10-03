#include <iostream>
#include "wtf.hpp"


using namespace wtf::default_theme;

/*
struct MyForm : form{
  MyForm() : oTabs(*this){
    ResizedEvent.connect([this](wm_size_flags, int width, int height){
      oTabs.move(3, 3, width - 6, height - 6);
    });
    update();
  }
  tab_container oTabs;
};
*/

struct MyForm : form{
  MyForm() : oButton(*this){
    oButton.move(10, 10, 100, 25);
    oButton.text(_T("FNORD!!!"));
    oButton.button_type(button::button_types::toggle);
    oButton.ClickEvent.connect([this](const wtf::point&){
      if (oButton.button_state() == button::button_states::up){
      } else{

      }
    });

    auto & iText = oButton.get_policy<wtf::has_text>();


    std::cout << typeid(iText).name() << std::endl;

  }

  button oButton;
};

/*

struct MyForm : form{
  MyForm() : oText(*this){
    oText.move(10, 10, 100, 25);
    oText.text(_T("FNORD!!!"));
  }

  textbox oText;
};
*/

int main(){
  try{
    MyForm oForm;
    oForm.show();
    wtf::message oMsg;
    return oMsg.pump();
  }
  catch (const wtf::exception& ex){
    std::cout << ex.what() << std::endl;
  }
  return -1;
}

