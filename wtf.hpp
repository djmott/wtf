#pragma once
#include <Windows.h>
#include <gdiplus.h>

#include <stdexcept>
#include <string>
#include <cassert>

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

namespace wtf{
  template <typename _ImplT, template <WNDPROC> class _WindowClassT, template <typename> class ... _PolicyListT> struct window;
}

#include "exception.hpp"
#include "message.hpp"
#include "icon.hpp"
#include "cursor.hpp"
#include "brush.hpp"
#include "menu.hpp"

#include "window_class_ex.hpp"
#include "window.hpp"
#include "message_handlers.hpp"
#include "form.hpp"
#include "button.hpp"