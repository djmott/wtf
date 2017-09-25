/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#define NOMINMAX 1

#if !defined(__WTF_DEBUG_MESSAGES__)
  #define __WTF_DEBUG_MESSAGES__ 0
#endif

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
#include <sstream>
#include <mutex>
#include <locale>
#if (__WTF_DEBUG_MESSAGES__)
#include <iostream>
#endif

/** @namespace wtf
Primary namespace
*/
namespace wtf {


  /** @enum coord_frame
  Distinguishes coordinate frame relative to the screen or window client area  
  */
  enum class coord_frame {
    screen, /**< screen relative */
    client, /**< window client area relative */
  };

  struct window;
  

  /** @typedef tstring
  Primary string representation based on stl. Can be either MULTIBYTE or UNICODE depending on compilation mode.
  */
  using tstring = std::basic_string<TCHAR>;


  /** @namespace wtf::_
  Hidden namespace for internal structures and algorithms not for external consumption
  */
  namespace _ {
    static std::mutex& _active_forms_lock() {
      static std::mutex _forms_lock;
      return _forms_lock;
    }

    static std::vector<const window*>& _active_forms() {
      static std::vector<const window*> _forms;
      return _forms;
    }

    extern "C" HINSTANCE__ __ImageBase;
    inline static HINSTANCE instance_handle(){ return &__ImageBase; }

    template <typename, int> struct to_tstring_impl;

    template <typename _Ty> struct to_tstring_impl<_Ty, sizeof(wchar_t)>{
      static tstring get(const _Ty& value){ return std::to_wstring(value); }
    };
    template <typename _Ty> struct to_tstring_impl<_Ty, sizeof(char)>{
      static tstring get(const _Ty& value){ return std::to_string(value); }
    };

    template <template <class> class ... _Policies> struct policy_list;

    template <template <class> class> struct policy_traits{
      using requires = policy_list<>;
    };

  }


  template <typename _Ty> static tstring to_tstring(_Ty value){ return _::to_tstring_impl<_Ty, sizeof(TCHAR)>::get(value); }

  /** @namespace wtf::policy
  Contains the behavioral policies
  */
  namespace policy{}

  static const std::vector<const window*>& active_forms() { return _::_active_forms(); }


}

/**
@defgroup UI Widgets
*/

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


#include "_/window_message.hpp"
#include "_/window_class_ex.hpp"
#include "_/device_context.hpp"
#include "_/msg_names.hpp"
#include "_/weak_enum.hpp"
#include "_/text_metrics.hpp"
#include "_/paint_struct.hpp"
#include "_/SystemParameters.hpp"
#include "_/message.hpp"
#include "_/policy_list.hpp"
#include "_/effects.hpp"


#include "window.hpp"
#include "window_impl.hpp"

#include "messages/messages.hpp"
#include "messages/wm_activate.hpp"
#include "messages/wm_char.hpp"
#include "messages/wm_close.hpp"
#include "messages/wm_create.hpp"
#include "messages/wm_dblclick.hpp"
#include "messages/wm_destroy.hpp"
#include "messages/wm_enable.hpp"
#include "messages/wm_erasebkgnd.hpp"
#include "messages/wm_geticon.hpp"
#include "messages/wm_keydown.hpp"
#include "messages/wm_keyup.hpp"
#include "messages/wm_killfocus.hpp"
#include "messages/wm_mouse_down.hpp"
#include "messages/wm_mouse_leave.hpp"
#include "messages/wm_mouse_move.hpp"
#include "messages/wm_mouse_up.hpp"
#include "messages/wm_mouse_wheel.hpp"
#include "messages/wm_move.hpp"
#include "messages/wm_nccalcsize.hpp"
#include "messages/wm_ncmouse_down.hpp"
#include "messages/wm_ncmouse_leave.hpp"
#include "messages/wm_ncmouse_move.hpp"
#include "messages/wm_ncmouse_up.hpp"
#include "messages/wm_ncpaint.hpp"
#include "messages/wm_paint.hpp"
#include "messages/wm_setcursor.hpp"
#include "messages/wm_setfocus.hpp"
#include "messages/wm_showwindow.hpp"
#include "messages/wm_size.hpp"
#include "messages/wm_sizing.hpp"
#include "messages/wm_timer.hpp"



#include "policies/has_background.hpp"
#include "policies/has_border.hpp"
#include "policies/has_caret.hpp"
#include "policies/has_click.hpp"
#include "policies/has_close.hpp"
#include "policies/has_cursor.hpp"
#include "policies/has_enable.hpp"
#include "policies/has_focus.hpp"
#include "policies/has_font.hpp"
#include "policies/has_icon.hpp"
#include "policies/has_invalidate.hpp"
#include "policies/has_move.hpp"
#include "policies/has_orientation.hpp"
#include "policies/has_timer.hpp"
#include "policies/has_repeat_click.hpp"
#include "policies/has_show.hpp"
#include "policies/has_size.hpp"
#include "policies/has_text.hpp"
#include "policies/has_titlebar.hpp"
#include "policies/has_zorder.hpp"


#include "ui/panel.hpp"
#include "ui/label.hpp"
#include "ui/button.hpp"
#include "ui/scroll_bar.hpp"
#include "ui/checkbox.hpp"
#include "ui/form.hpp"
#include "ui/scroll_panel.hpp"
#include "ui/inputbox.hpp"
#include "ui/list.hpp"
#include "ui/menu.hpp"
#include "ui/progress_bar.hpp"
#include "ui/split_container.hpp"
#include "ui/tab_container.hpp"
#include "ui/textbox.hpp"
#include "ui/toggle_button.hpp"
#include "ui/tree.hpp"
