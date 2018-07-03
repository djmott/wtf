/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace dialogs {
    struct choose_color : CHOOSECOLOR {
      choose_color() {
        memset(this, 0, sizeof(CHOOSECOLOR));
        lStructSize = sizeof(CHOOSECOLOR);
        lpCustColors = _custom;
        Flags = CC_FULLOPEN;
      }
      bool run(rgb& ret) {
        if (!ChooseColor(this)) return false;
        ret = rgb(rgbResult);
        return true;
      }
    protected:
      COLORREF _custom[16];
    };
  }
}