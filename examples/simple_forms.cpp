/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#include "wtf/wtf.hpp"

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  return wtf::form().run();
}
