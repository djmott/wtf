﻿/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#if !defined(NOMINMAX)
  #define NOMINMAX 1
#endif

#if !defined(_WIN32_IE)
  #define _WIN32_IE 0x600
#endif

/** @def WTF_USE_VISUAL_STYLES
@brief Enables/disables the use of visual styles through uxtheme.dll
*/
#if !defined(WTF_USE_VISUAL_STYLES)
  #define WTF_USE_VISUAL_STYLES 1
#endif

/** @def WTF_USE_COMMON_CONTROLS
@brief Enables/disables the use and dependency of extended native controls in comctl32.dll
@details The most basic set of controls are implemented in the Windows GDI subsystem and extended in the common controls library
*/
#if !defined(WTF_USE_COMMON_CONTROLS)
  #define WTF_USE_COMMON_CONTROLS 1
#endif

/** @def WTF_USE_RICHEDIT
@brief Enables/disables the use and dependency of the rich edit control in msftedit.dll
*/
#if !defined(WTF_USE_RICHEDIT)
  #define WTF_USE_RICHEDIT 1
#endif

/** @def WTF_USE_COMMON_DIALOGS
@brief Enables/disables the use and dependency of common dialogs implemented in comdlg32.dll
*/
#if !defined(WTF_USE_COMMON_DIALOGS)
  #define WTF_USE_COMMON_DIALOGS 1
#endif

/** @def WTF_DEBUG_MESSAGES
@brief Debug windows messages with OutputDebugString
*/
#if !defined(WTF_DEBUG_MESSAGES)
  #if defined(DEBUG) || defined(_DEBUG)
    #define WTF_DEBUG_MESSAGES 1
  #else
    #define WTF_DEBUG_MESSAGES 0
  #endif
#endif


#define OEMRESOURCE

#include <tchar.h>
#include <WinSock2.h>
#include <Windows.h>
#include <windowsx.h>

#if WTF_USE_VISUAL_STYLES
  #pragma comment(lib, "uxtheme.lib")
  #include <Uxtheme.h>
  #if defined _M_IX86
    #pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
  #elif defined _M_IA64
    #pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
  #elif defined _M_X64
    #pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
  #else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
  #endif
#endif


#if WTF_USE_COMMON_CONTROLS
  #include <CommCtrl.h>
  #pragma comment(lib, "comctl32.lib")
#endif

#if WTF_USE_RICHEDIT
  #include <richedit.h>
#endif

#if WTF_USE_COMMON_DIALOGS
  #include <commdlg.h>
  #include <cderr.h>
  #pragma comment(lib, "comdlg32.lib")
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

#if !defined(DOXY_INVOKED)
  #define DOXY_INVOKED 0
#endif


/** @defgroup Policies Policies
@brief Behavioral policies
@details This group of classes are discrete behavioral components. They're composed to create feature-rich concrete controls
*/

/** @defgroup Messages Messages
@brief Message handling policies
@details This group of policy classes handle windows messages. They're composed to create feature-rich concrete controls
*/

/** @defgroup Controls Controls
@brief WTF Controls
@details This group of concrete classes are a composition of behavioral policies and message handlers
*/

/** @namespace wtf
@brief Primary namespace
*/
namespace wtf {

  /** @namespace wtf::controls
  @brief Native controls
  @details These controls are implemented by the Windows system
  */
  namespace controls {}

  /** @namespace wtf::custom
  @brief Custom controls
  @details These controls are implemented by WTF and only available in WTF binaries
  */
  namespace custom{}

  /** @namespace wtf::layouts
  @brief Layout policies
  @details These policies define how child windows are positioned and resized
  */
  namespace layouts {}

  /**
  @interface window window.hpp
  @brief Base class of controls and forms
  @details This class is inherited as the super-most base class of controls and forms via hierarchy generation.
  */
  struct window;
  
  /**
  @class window_impl window.hpp
  @brief Implements a control or form
  @details This is the hierarchy generator that composes a collection of behavior policies and message handlers into a concrete control or form. 
  @details Policies are combine in a linear hierarchy in the order listed with the window class being the super-most base class.
  @tparam implementation_type The concrete implementation
  @tparam policy_list The list of behavioral policies that the implementation will inherit.
  */
  template <typename implementation_type, template <typename> typename...policy_list> struct window_impl;

  /** @enum coord_frame
  @brief Distinguishes coordinate frame relative to the screen or window client area
  */
  enum class coord_frame {
    screen, /**< screen relative */
    client, /**< window client area relative */
  };

  /** @typedef tstring
  @brief Primary string representation. Can be either MULTIBYTE or UNICODE depending on compilation mode.
  */
  using tstring = std::basic_string<TCHAR>;

#if !DOXY_INVOKED
  extern "C" HINSTANCE__ __ImageBase;
#endif

  //! @brief Returns the instance handle of the current process.
  inline static constexpr HINSTANCE instance_handle() noexcept { return &__ImageBase; }



  /** @brief Constructs a tstring representation of a value
  @param value value to convert
  @return a tstring representation of the value
  */
  template <typename _Ty> static tstring to_tstring(_Ty value){ return _::to_tstring_impl<_Ty, TCHAR>::get(value); }

  /** @namespace wtf::policy
  @brief Behavioral policies
  @ingroup Policies
  */
  namespace policy{
#if !DOXY_INVOKED
    namespace _{}
#endif
  }

#if !DOXY_INVOKED
  namespace _ {
    static std::mutex& _active_forms_lock() noexcept {
      static std::mutex _forms_lock;
      return _forms_lock;
    }

    static std::vector<const window*>& _active_forms() noexcept {
      static std::vector<const window*> _forms;
      return _forms;
    }
  }
#endif

  //! @brief Contains the active WTF forms in the process
  static const std::vector<const window*>& active_forms()  noexcept { return _::_active_forms(); }

#if !DOXY_INVOKED
  namespace _ {

    template <typename, typename> struct to_tstring_impl;

    template <typename _Ty> struct to_tstring_impl<_Ty, wchar_t> {
      static std::basic_string<wchar_t> get(const _Ty& value) { return std::to_wstring(value); }
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
#endif

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
#include "bitmap.hpp"
#include "brush.hpp"
#include "pen.hpp"
#include "region.hpp"
#include "resource.hpp"
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
#include "messages/nm_click.hpp"
#include "messages/nm_dblclick.hpp"
#include "messages/nm_killfocus.hpp"
#include "messages/nm_rclick.hpp"
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

#include "layouts/grid.hpp"

#include "controls/button.hpp"
#include "controls/combobox.hpp"
#include "controls/edit.hpp"
#include "controls/label.hpp"
#include "controls/listbox.hpp"
#include "controls/menu.hpp"

#if WTF_USE_COMMON_CONTROLS
  #include "controls/calendar.hpp"
  #include "controls/date_time.hpp"
  #include "controls/hotkey.hpp"
  #include "controls/image_list.hpp"
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

#if WTF_USE_COMMON_DIALOGS
  #include "dialogs/exception.hpp"
  #include "dialogs/choose_color.hpp"
  #include "dialogs/file_open_save.hpp"
#endif

#include "custom/splitter.hpp"
#include "custom/panel.hpp"

#include "form.hpp"
#include "console.hpp"