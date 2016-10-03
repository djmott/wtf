#pragma once

#define NOMINMAX 1

#include <Windows.h>
#include <windowsx.h>
#include <tchar.h>

#include <algorithm>
#include <stdexcept>
#include <string>
#include <cassert>
#include <typeinfo>
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include <iterator>

//#define __F OutputDebugStringA
#define __F(...)

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

namespace wtf{
  using tstring = std::basic_string<TCHAR>;
  template <typename, template <typename> typename ...> struct window;


  template <template <typename> typename ...> struct policy_list;

  template <template <typename> typename _HeadT, template <typename> typename ..._TailT> 
  struct policy_list<_HeadT, _TailT...> : policy_list<_TailT...>{};

  template <> struct policy_list<>{};

}

#include "msg_names.hpp"
#include "weak_enum_class.hpp"
#include "exception.hpp"
#include "message_box.hpp"
#include "callback.hpp"
#include "SystemParameters.hpp"
#include "point.hpp"
#include "color.hpp"
#include "rect.hpp"
#include "size.hpp"
#include "paint_struct.hpp"
#include "message.hpp"
#include "icon.hpp"
#include "cursor.hpp"
#include "brush.hpp"
#include "pen.hpp"
#include "region.hpp"
#include "font.hpp"
#include "device_context.hpp"
#include "menu.hpp"
#include "window.hpp"
#include "behaviors.hpp"

#include "themes/default/button.hpp"
#include "themes/default/form.hpp"
#include "themes/default/label.hpp"
#include "themes/default/scroll_bar.hpp"
#include "themes/default/panel.hpp"
#include "themes/default/listbox.hpp"
#include "themes/default/textbox.hpp"
#include "themes/default/tab_container.hpp"
