#include "wtf/wtf.hpp"

#if defined(__WTF_DEBUG_MESSAGES__)
int main() {
#else
int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#endif
  return wtf::form().run();
}
