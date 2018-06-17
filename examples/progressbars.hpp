#include "wtf/wtf.hpp"
using namespace wtf;

struct ProgressBars : wtf::controls::label {

  ProgressBars() : wtf::controls::label(), _hprogressbar(), _vprogressbar(), _hmarquee(){
    add(_hprogressbar);
    add(_vprogressbar);
    add(_hmarquee);

    OnCreated += [this](...) {
      _hprogressbar.set_range(0, 100);
      _hprogressbar.value(50);
      _vprogressbar.set_range(0, 100);
      _vprogressbar.value(50);
      _hmarquee.set_marque(true);
    };
    OnSize += [this](...) {
      _hprogressbar.move(10, 10, this->width() - 20, 25);
      _vprogressbar.move(10, 40, 200, this->height() - 60);
      _hmarquee.move(220, 40, this->width() - 230, 25);
    };
  }
  controls::progressbar<orientations::horizontal, false> _hprogressbar;
  controls::progressbar<orientations::vertical, true> _vprogressbar;
  controls::marquee<orientations::horizontal, true> _hmarquee;

};
