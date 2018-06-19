/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {

    namespace _ {


      template <typename _impl_t> using button_super_t = window_impl<_impl_t,
        policy::has_text,
        policy::has_font,
        policy::has_enable,
        policy::has_invalidate,
        policy::has_move,
        policy::wm_command
      >;
    }


    /** @class button
    @ingroup Widgets
    @brief A standard clickable push style button.
    */
    struct button : _::button_super_t<button> {};
  

    /** @class checkbox
    @ingroup Widgets
    @brief A dual state (boolean) check box.
    */
    struct checkbox : _::button_super_t<checkbox> {
      static constexpr DWORD Style = window::Style | BS_AUTOCHECKBOX;
    };


    /** @class radio_group
    @ingroup Widgets
    @brief Starts a group of radio_button items.
    */
    struct radio_group : _::button_super_t<radio_group> {
      static constexpr DWORD Style = window::Style | BS_AUTORADIOBUTTON | WS_GROUP;
    };


    /** @class radio_button
    @ingroup Widgets
    @brief An element in a radio_group. Only a single item in a radio_group can be selected at one time
    */
    struct radio_button : _::button_super_t<radio_button> {
      static constexpr DWORD Style = window::Style | BS_AUTORADIOBUTTON;
    };

    /** @class tristate
    @ingroup Widgets
    @brief A tri-state check box.
    */
    struct tristate : _::button_super_t<tristate> {
      static constexpr DWORD Style = window::Style | BS_AUTO3STATE;
    };

  }
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

}
