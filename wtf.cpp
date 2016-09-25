
#include <iostream>
#include "wtf.hpp"

namespace theme = wtf::matrix_theme;

struct MyForm : theme::form{
  MyForm() : form(), oButton(*this) {
    titlebar_text(L"MyForm");
    oButton.move(25, 25, 100, 50);
    oButton.label(L"Click me");
    oButton.BackgroundBrush(Gdiplus::SolidBrush(GetSysColor(RGB(255, 0, 0))));
  }
  theme::button oButton;
};

int main(){
  try{
    MyForm oForm;
    oForm.show();
    wtf::message oMsg;
    return oMsg.pump();
  }
  catch (const wtf::exception& ex){
    std::cerr << "A WTF exception occurred in " << ex.file() << "(" << ex.line() << ") : " << ex.code() << std::endl << ex.what() << std::endl;
  }
  return -1;
}