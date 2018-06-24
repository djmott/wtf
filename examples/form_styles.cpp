/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/

#include "wtf/wtf.hpp"

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  wtf::message_box::exec(nullptr, _T("A standard resizable form will now display"));
  wtf::form().run();
  wtf::message_box::exec(nullptr, _T("A fixed popup dialog will now display"));
  wtf::dialog().run();
  wtf::message_box::exec(nullptr, _T("A sizeable tool window will now display"));
  return wtf::tool_window().run();
}
