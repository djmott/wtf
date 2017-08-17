#include "wtf/wtf.hpp"

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int){
  return wtf::form().run();
}