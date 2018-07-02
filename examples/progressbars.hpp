/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/

#include "wtf/wtf.hpp"
using namespace wtf;

struct ProgressBars : wtf::controls::label {

  ProgressBars() : wtf::controls::label(), _hprogressbar(), _vprogressbar(){
    add(_hprogressbar);
    add(_vprogressbar);

    OnCreated += [this](...) {
      _hprogressbar.set_range(0, 100);
      _hprogressbar.value(50);
      _vprogressbar.set_range(0, 100);
      _vprogressbar.value(50);
    };
    OnSize += [this](...) {
      _hprogressbar.move(10, 10, this->width() - 20, 25);
      _vprogressbar.move(10, 40, 200, this->height() - 60);
    };
  }
  controls::hprogressbar _hprogressbar;
  controls::vprogressbar _vprogressbar;

};
