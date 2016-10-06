#include <iostream>
#include "wtf.hpp"


struct main_form : wtf::form{
  main_form() :
    wtf::form(),
    oRightAligned(*this),
    oLeftAligned(*this),
    oCenterAligned(*this),
    oHugeFont(*this)
  {
    titlebar("Label example");
    oRightAligned.move(5, 5, 200, 20);
    oRightAligned.border_style(wtf::label::border_styles::flat);
    oRightAligned.text("Right aligned");
    oRightAligned.text_horizontal_alignment(wtf::label::text_horizontal_alignments::right);

    oLeftAligned.move(5, oRightAligned.top() + oRightAligned.height() + 5, oRightAligned.width(), oRightAligned.height());
    oLeftAligned.border_style(wtf::label::border_styles::flat);
    oLeftAligned.text("Left aligned");
    oLeftAligned.text_horizontal_alignment(wtf::label::text_horizontal_alignments::left);

    oCenterAligned.move(5, oLeftAligned.top() + oLeftAligned.height() + 5, oLeftAligned.width(), oLeftAligned.height());
    oCenterAligned.border_style(wtf::label::border_styles::flat);
    oCenterAligned.text("Center aligned");
    oCenterAligned.text_horizontal_alignment(wtf::label::text_horizontal_alignments::center);
    oCenterAligned.fore_color(wtf::rgb(0, 0, 100));
    oCenterAligned.font().italic(true);
    oCenterAligned.font().height(20);

    oHugeFont.move(5, oCenterAligned.top() + oCenterAligned.height() + 5, oCenterAligned.width(), oCenterAligned.height());
    oHugeFont.text("Large text");
    oHugeFont.border_style(wtf::label::border_styles::flat);
    oHugeFont.font().height(35);
  }

  wtf::label oRightAligned;
  wtf::label oLeftAligned;
  wtf::label oCenterAligned;
  wtf::label oHugeFont;
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