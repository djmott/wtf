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


namespace wtf{

  extern "C" HINSTANCE__ __ImageBase;
  inline static HINSTANCE instance_handle(){ return &__ImageBase; }

  using tstring = std::basic_string<TCHAR>;
  template <typename, template <typename> class ...> struct window;

  namespace policy{
    template <template <typename> class> struct traits;

    template <template <typename> class ...> struct list;

    template <template <typename> class _HeadT, template <typename> class ..._TailT>
    struct list<_HeadT, _TailT...> : list<_TailT...>{};

    template <> struct list<>{};
  }
  namespace _{
    template <typename, template <typename> class ... > struct base_window;
  }
}


#include "exception.hpp"
#include "message_box.hpp"
#include "callback.hpp"
#include "point.hpp"
#include "color.hpp"
#include "rect.hpp"
#include "size.hpp"
#include "icon.hpp"
#include "cursor.hpp"
#include "brush.hpp"
#include "pen.hpp"
#include "region.hpp"
#include "font.hpp"

#include "detail/window_class_ex.hpp"
#include "detail/msg_names.hpp"
#include "detail/weak_enum_class.hpp"
#include "detail/device_context.hpp"
#include "detail/text_metrics.hpp"
#include "detail/paint_struct.hpp"
#include "detail/base_window.hpp"
#include "detail/SystemParameters.hpp"
#include "detail/message.hpp"


#include "policies/has_border.hpp"
#include "policies/has_caret.hpp"
#include "policies/has_click.hpp"
#include "policies/has_close.hpp"
#include "policies/has_cursor.hpp"
#include "policies/has_focus.hpp"
#include "policies/has_font.hpp"
#include "policies/has_icon.hpp"
#include "policies/has_keyboard.hpp"
#include "policies/has_mouse.hpp"
#include "policies/has_paint.hpp"
#include "policies/has_show.hpp"
#include "policies/has_size.hpp"
#include "policies/has_text.hpp"
#include "policies/has_timer.hpp"
#include "policies/has_titlebar.hpp"
#include "policies/policy_traits.hpp"


#include "ui/window.hpp"
#include "ui/menu.hpp"
#include "ui/form.hpp"
#include "ui/label.hpp"
#include "ui/push_button.hpp"
#include "ui/scroll_bar.hpp"
#include "ui/panel.hpp"
#include "ui/listbox.hpp"
#include "ui/textbox.hpp"
#include "ui/toggle_button.hpp"
#include "ui/tab_container.hpp"
