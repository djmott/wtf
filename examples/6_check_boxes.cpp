#include "wtf/wtf.hpp"

using namespace wtf;

struct frmMain : form {
  frmMain() : form(), _left(this), _right(this){
    _left.text_horizontal_alignment(text_horizontal_alignments::left);
    _right.text_horizontal_alignment(text_horizontal_alignments::right);

    OnCreated += [this](...) {
      _left.text("Left Aligned");
      _right.text("Right Aligned");
      _left.move(5, 5, 100, 25);
      _right.move(5, 40, 100, 25);
    };

  }

  checkbox _left, _right;

};

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  return frmMain().run();
  return 0;
}