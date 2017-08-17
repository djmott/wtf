#include "wtf/wtf.hpp"

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  wtf::message_box::exec(nullptr, _T("A standard resizable form will now display"));
  wtf::form().run();
  wtf::message_box::exec(nullptr, _T("A fixed popup dialog will now display"));
  wtf::dialog().run();
  wtf::message_box::exec(nullptr, _T("A sizeable tool window will now display"));
  return wtf::tool_window().run();
}