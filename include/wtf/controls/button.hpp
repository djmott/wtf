﻿/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#define DOXY_INHERIT_BUTTON_SUPER \
  DOXY_INHERIT_WINDOW \
  DOXY_INHERIT_HAS_TEXT \
  DOXY_INHERIT_HAS_FONT \
  DOXY_INHERIT_HAS_ENABLE \
  DOXY_INHERIT_HAS_INVALIDATE \
  DOXY_INHERIT_HAS_MOVE \
  DOXY_INHERIT_HAS_STYLE \
  DOXY_INHERIT_WM_COMMAND

namespace wtf {
  namespace controls {

    /** @class button
    @brief A standard clickable push style button.
    @ingroup Widgets
    @image html button.png
    */
    struct button : DOXY_INHERIT_BUTTON_SUPER window_impl<button,
      policy::has_text,
      policy::has_font,
      policy::has_enable,
      policy::has_invalidate,
      policy::has_move,
      policy::has_style,
      policy::wm_command
    > {
      size ideal_size() const {
        size oRet;
        wtf::exception::throw_lasterr_if(::SendMessage(*this, BCM_GETIDEALSIZE, 0, reinterpret_cast<LPARAM>(&oRet)), [](LRESULT l) { return !l; });
        return oRet;
      }
    };
  

    /** @class checkbox
    @brief A dual state (boolean) check box.
    @ingroup Widgets
    @image html checkbox.png
    */
    struct checkbox : button {
      static constexpr DWORD Style = window::Style | BS_AUTOCHECKBOX;
    };


    /** @class radio_group
    @brief A radio_button that starts a group.
    @details Only a single radio_button in a radio_group may be selected at a time
    @ingroup Widgets
    @image html radio_button.png
    */
    struct radio_group : button {
      static constexpr DWORD Style = window::Style | BS_AUTORADIOBUTTON | WS_GROUP;
    };


    /** @class radio_button
    @brief An element in a radio_group. Only a single item in a radio_group can be selected at one time
    @ingroup Widgets
    @image html radio_button.png
    */
    struct radio_button : button {
      static constexpr DWORD Style = window::Style | BS_AUTORADIOBUTTON;
    };

    /** @class tristate
    @brief A tri-state check box.
    @details A tristate is similar to a checkbox. It has an on-off state like a checkbox plus a third greyed state.
    @ingroup Widgets
     @image html tristate.png
   */
    struct tristate : button {
      static constexpr DWORD Style = window::Style | BS_AUTO3STATE;
    };

  }

#if !DOXY_INVOKED
  namespace _ {
#if WTF_USE_COMMON_CONTROLS
    TCHAR sWC_BUTTON[] = WC_BUTTON;
#else
    TCHAR sWC_BUTTON[] = _T("BUTTON");
#endif
  }

  template <WNDPROC window_proc> struct window_class<controls::button, window_proc> : super_window_class<_::sWC_BUTTON, controls::button, window_proc> {};
  template <WNDPROC window_proc> struct window_class<controls::checkbox, window_proc> : super_window_class<_::sWC_BUTTON, controls::checkbox, window_proc> {};
  template <WNDPROC window_proc> struct window_class<controls::radio_button, window_proc> : super_window_class<_::sWC_BUTTON, controls::radio_button, window_proc> {};
  template <WNDPROC window_proc> struct window_class<controls::radio_group, window_proc> : super_window_class<_::sWC_BUTTON, controls::radio_group, window_proc> {};
  template <WNDPROC window_proc> struct window_class<controls::tristate, window_proc> : super_window_class<_::sWC_BUTTON, controls::tristate, window_proc> {};
#endif
}
