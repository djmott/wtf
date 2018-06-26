/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#if !defined(NOMINMAX)
  #define NOMINMAX 1
#endif

#if !defined(_WIN32_IE)
  #define _WIN32_IE 0x600
#endif


#if !defined(WTF_USE_VISUAL_STYLES)
  #define WTF_USE_VISUAL_STYLES 0
#endif


#if !defined(WTF_USE_COMMON_CONTROLS)
  #define WTF_USE_COMMON_CONTROLS 1
#endif

#if !defined(WTF_USE_RICHEDIT)
#define WTF_USE_RICHEDIT 0
#endif

#if !defined(WTF_DEBUG_MESSAGES)
  #if defined(DEBUG) || defined(_DEBUG)
    #define WTF_DEBUG_MESSAGES 1
  #else
    #define WTF_DEBUG_MESSAGES 0
  #endif
#endif

#include <tchar.h>
#include <Windows.h>
#include <windowsx.h>

#if WTF_USE_VISUAL_STYLES
  #pragma comment(lib, "uxtheme.lib")
  #include <Uxtheme.h>
  #pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif


#if WTF_USE_COMMON_CONTROLS
  #include <CommCtrl.h>
  #pragma comment(lib, "comctl32.lib")
#endif

#if WTF_USE_RICHEDIT
  #include <richedit.h>
#endif

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
#include <codecvt>
#include <atomic>


/** @defgroup Widgets Widgets
WTF Visual components
*/
/** @defgroup Policies Policies
Behavioral policies of widgets
*/

/** @defgroup Messages Messages
Message processing policies of widgets
*/

/** @namespace wtf
Primary namespace
*/
namespace wtf {
  /**
  * @cond PRIVATE
  * @namespace wtf::_
  * Hidden namespace for internal structures and algorithms not for external consumption
  */
  namespace _ {}
  //!@endcond

  /** @namespace wtf::controls
  Native Windows controls
  */
  namespace controls {
    /** @cond PRIVATE
     * @namespace _
     * Internal namespace
     */
    namespace _{}
    //! @endcond

  }

  /**
  @interface window window.hpp
  @brief Base class of widgets and forms
  This class is inherited as the super-most base class of widgets and forms via hierarchy generation.
  */
  struct window;
  
  /**
  @class window_impl window.hpp
  @brief Implements a widgets or form
  This is the hierarchy generator that composes a list of behavior patterns into a concrete widget or form. 
  Policies are combine in a linear hierarchy in the order listed with the window class being the super-most base class.
  @tparam implementation_type The concrete implementation
  @tparam policy_list The list of behavioral policies that the implementation will inherit.
  */
  template <typename implementation_type, template <typename> typename...policy_list> struct window_impl;

  /** @enum coord_frame
  Distinguishes coordinate frame relative to the screen or window client area
  */
  enum class coord_frame {
    screen, /**< screen relative */
    client, /**< window client area relative */
  };

  /** @typedef tstring
  Primary string representation. Can be either MULTIBYTE or UNICODE depending on compilation mode.
  */
  using tstring = std::basic_string<TCHAR>;

  //!@cond PRIVATE
  extern "C" HINSTANCE__ __ImageBase;
  //!@endcond

  /** @fn HINSTANCE instance_handle()
  Returns the instance handle of the current process.
  */
  inline static constexpr HINSTANCE instance_handle() noexcept { return &__ImageBase; }

  /** 
  @cond PRIVATE
  */
  namespace _ {
    static std::mutex& _active_forms_lock() noexcept {
      static std::mutex _forms_lock;
      return _forms_lock;
    }

    static std::vector<const window*>& _active_forms() noexcept {
      static std::vector<const window*> _forms;
      return _forms;
    }

    template <typename, typename> struct to_tstring_impl;

    template <typename _Ty> struct to_tstring_impl<_Ty, wchar_t>{
      static std::basic_string<wchar_t> get(const _Ty& value){ return std::to_wstring(value); }
    };
    template <typename _Ty> struct to_tstring_impl<_Ty, char> {
      static std::basic_string<char> get(const _Ty& value) { return std::to_string(value); }
    };
    template <> struct to_tstring_impl<const char *, char> {
      static std::basic_string<char> get(const char * value) { return std::basic_string<char>(value); }
    };
    template <> struct to_tstring_impl<const char *, wchar_t> {
      static std::basic_string<wchar_t> get(const char * value) {
        return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(value);
      }
    };
    template <> struct to_tstring_impl<std::string, char> {
      static std::basic_string<char> get(std::string value) {
        return std::basic_string<char>(value);
      }
    };
    template <> struct to_tstring_impl<std::string, wchar_t> {
      static std::basic_string<wchar_t> get(std::string value) {
        return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(value);
      }
    };

  }
  //!@endcond

  /**
  @fn tstring to_tstring(_Ty value)
  Constructs a tstring representation of a value
  @param value value to convert
  @return a tstring representation of the value
  */
  template <typename _Ty> static tstring to_tstring(_Ty value){ return _::to_tstring_impl<_Ty, TCHAR>::get(value); }

  /** @namespace wtf::policy
  Behavioral policies
  @ingroup Policies
  */
  namespace policy{
    /**
    @cond PRIVATE
    @namespace wtf::_
    Hidden namespace for internal structures and algorithms not for external consumption
    */
    namespace _{}
    //!@endcond

  }

  static const std::vector<const window*>& active_forms()  noexcept { return _::_active_forms(); }

}

#if !defined(PRAGMA_)
  #define PRAGMA_( ... ) __pragma( __VA_ARGS__ )
#endif
#if !defined(NOVTABLE)
  #define NOVTABLE __declspec(novtable)
#endif
#if !defined(_QUOTE)
  #define _QUOTE( ... ) # __VA_ARGS__
#endif
#if !defined(QUOTE)
  #define QUOTE( ... ) _QUOTE( __VA_ARGS__ )
#endif
#if !defined(NOTE)
  #define NOTE( ... ) PRAGMA_(message ( __FILE__ "(" QUOTE(__LINE__) "): TODO : " __VA_ARGS__ ))
#endif
#if !defined(NOTE)
  #define NOTE( ... ) PRAGMA_(message ( __FILE__ "(" QUOTE(__LINE__) "): NOTE : " __VA_ARGS__ ))
#endif

#if defined(_DEBUG)
  #if !defined(D_)
    #define D_(...) __VA_ARGS__
  #endif
  #if !defined(R_)
    #define R_(...)
  #endif
#else
  #if !defined(D_)
    #define D_(...)
  #endif
  #if !defined(R_)
    #define R_(...) __VA_ARGS__
  #endif
#endif

#include "exception.hpp"

#include "_/meta.hpp"
#include "_/msg_names.hpp"
#include "_/weak_enum.hpp"
#include "_/coinitialize.hpp"
#include "_/init_common_controls.hpp"

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
#include "window_class.hpp"
#include "device_context.hpp"
#include "paint_struct.hpp"
#include "message.hpp"
#include "window_message.hpp"
#include "menu.hpp"
#include "window.hpp"
#include "monitor.hpp"

#include "_/effects.hpp"
#include "_/text_metrics.hpp"
#include "_/SystemParameters.hpp"

#include "messages/messages.hpp"
#include "messages/nm_killfocus.hpp"
#include "messages/nm_setfocus.hpp"
#include "messages/wm_activate.hpp"
#include "messages/wm_char.hpp"
#include "messages/wm_close.hpp"
#include "messages/wm_command.hpp"
#include "messages/wm_create.hpp"
#include "messages/wm_dblclick.hpp"
#include "messages/wm_destroy.hpp"
#include "messages/wm_enable.hpp"
#include "messages/wm_erasebkgnd.hpp"
#include "messages/wm_geticon.hpp"
#include "messages/wm_getminmaxinfo.hpp"
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
#include "messages/wm_notify.hpp"
#include "messages/wm_notifyformat.hpp"
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
#include "policies/has_exstyle.hpp"
#include "policies/has_focus.hpp"
#include "policies/has_font.hpp"
#include "policies/has_hscroll.hpp"
#include "policies/has_icon.hpp"
#include "policies/has_image.hpp"
#include "policies/has_invalidate.hpp"
#include "policies/has_move.hpp"
#include "policies/has_orientation.hpp"
#include "policies/has_owner_drawn_border.hpp"
#include "policies/has_owner_drawn_font.hpp"
#include "policies/has_owner_drawn_text.hpp"
#include "policies/has_repeat_click.hpp"
#include "policies/has_show.hpp"
#include "policies/has_size.hpp"
#include "policies/has_style.hpp"
#include "policies/has_text.hpp"
#include "policies/has_timer.hpp"
#include "policies/has_titlebar.hpp"
#include "policies/has_vscroll.hpp"
#include "policies/has_zorder.hpp"


#include "controls/button.hpp"
#include "controls/combobox.hpp"
#include "controls/edit.hpp"
#include "controls/label.hpp"
#include "controls/listbox.hpp"
#include "controls/menu.hpp"

#if WTF_USE_COMMON_CONTROLS
  #include "controls/avi_player.hpp"
  #include "controls/date_time.hpp"
  #include "controls/hotkey.hpp"
  #include "controls/image_list.hpp"
  #include "controls/ip_address.hpp"
  #include "controls/listview.hpp"
  #include "controls/pager.hpp"
  #include "controls/property_sheet.hpp"
  #include "controls/rebar.hpp"
  #include "controls/progressbar.hpp"
  #include "controls/tab.hpp"
  #include "controls/tree.hpp"
  #include "controls/statusbar.hpp"
  #include "controls/syslink.hpp"
  #include "controls/task_dialog.hpp"
  #include "controls/tooltip.hpp"
  #include "controls/trackbar.hpp"
  #include "controls/up_down.hpp"
#endif

#if WTF_USE_RICHEDIT
  #include "controls/richedit.hpp"
#endif

#include "custom/splitter.hpp"
#include "custom/panel.hpp"

#include "form.hpp"
