#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <gdiplus.h>

#include <stdexcept>
#include <string>
#include <cassert>
#include <typeinfo>
#include <memory>
#include <vector>

#define __F OutputDebugStringA

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

namespace wtf{
  struct empty{};
}

#include "exception.hpp"
#include "gdi_plus.hpp"
#include "message.hpp"
#include "icon.hpp"
#include "cursor.hpp"
#include "brush.hpp"
#include "menu.hpp"

#include "window_class_ex.hpp"
#include "window.hpp"
#include "message_handlers.hpp"
#include "behaviors.hpp"
#include "form.hpp"

#include "themes/system/button.hpp"
#include "themes/system/form.hpp"
#include "themes/system/label.hpp"

#include "themes/default/button.hpp"
#include "themes/default/form.hpp"
#include "themes/default/label.hpp"

#include "themes/matrix/button.hpp"
#include "themes/matrix/form.hpp"
#include "themes/matrix/label.hpp"

