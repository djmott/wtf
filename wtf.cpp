
#include <iostream>
#include "wtf.hpp"

using namespace wtf::default_theme;

struct MyForm : form{
  MyForm() : form(), oButton(*this), oLabel(*this){
    text(L"MyForm");
    BackgroundBrush(Gdiplus::SolidBrush(GetSysColor(RGB(0, 0, 255))));
    oButton.move(25, 25, 100, 50);
    oButton.text(L"Click me");
    oButton.BackgroundBrush(Gdiplus::SolidBrush(GetSysColor(RGB(255, 0, 0))));
    oLabel.move(200, 200, 200, 100);
    oButton.BackgroundBrush(Gdiplus::SolidBrush(GetSysColor(RGB(0, 255, 0))));
    //     oLabel.label_string(L"Help!");
  }
  button oButton;
  label oLabel;
};

int main(){
  try{
    wtf::default_theme::form oForm;
    oForm.show();
    wtf::message oMsg;
    return oMsg.pump();
  }
  catch (const wtf::exception& ex){
    std::cerr << "A WTF exception occurred in " << ex.file() << "(" << ex.line() << ") : " << ex.code() << std::endl << ex.what() << std::endl;
  }
  return -1;
}