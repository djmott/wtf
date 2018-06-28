/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {

  struct resource {
    enum class types {
      cursor = 1,
      bitmap = 2,
      icon = 3,
      menu = 4,
      dialog = 5,
      string = 6,
      fontdir = 7,
      font = 8,
      accelerator = 9,
      rcdata = 10,
      messagetable = 11,
      group_cursor = 12,
      group_icon = 14,
      version = 16,
      dlginclude = 17,
      plugplay = 19,
      vxd = 20,
      anicursor = 21,
      aniicon = 22,
      html = 23,
      manifest = 24,
    };
  };

}

