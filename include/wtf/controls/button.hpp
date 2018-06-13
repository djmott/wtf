/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {

    namespace policy {

      template <typename _super_t> struct isa_button : _super_t {
        template <typename ... _arg_ts> isa_button(_arg_ts&&...args) noexcept : 
          _super_t(std::forward<_arg_ts>(args)...) 
        {
          wtf::_::init_common_controls<wtf::_::standard_classes>::get();
        }
      };

      template <typename _impl_t> using button_super_t = window_impl<_impl_t,
        policy::isa_button,
        policy::has_text,
        policy::has_font,
        wtf::policy::has_invalidate,
        wtf::policy::has_move,
        wtf::policy::wm_command
      >;
    }


    struct button : policy::button_super_t<button> {
    };
  

    struct checkbox : policy::button_super_t<checkbox> {
      static constexpr DWORD Style = window::Style | BS_AUTOCHECKBOX;
    };


    struct radio_button : policy::button_super_t<radio_button> {
      static constexpr DWORD Style = window::Style | BS_AUTORADIOBUTTON;
    };


    struct radio_group : policy::button_super_t<radio_group> {
      static constexpr DWORD Style = window::Style | BS_AUTORADIOBUTTON | WS_GROUP;
    };

    struct tristate : policy::button_super_t<tristate> {
      static constexpr DWORD Style = window::Style | BS_AUTO3STATE;
    };

  }
  namespace _ { TCHAR sWC_BUTTON[] = WC_BUTTON; }

  template <WNDPROC window_proc> struct window_class<controls::button, window_proc> : super_window_class<_::sWC_BUTTON, controls::button, window_proc> {};
  template <WNDPROC window_proc> struct window_class<controls::checkbox, window_proc> : super_window_class<_::sWC_BUTTON, controls::checkbox, window_proc> {};
  template <WNDPROC window_proc> struct window_class<controls::radio_button, window_proc> : super_window_class<_::sWC_BUTTON, controls::radio_button, window_proc> {};
  template <WNDPROC window_proc> struct window_class<controls::radio_group, window_proc> : super_window_class<_::sWC_BUTTON, controls::radio_group, window_proc> {};
  template <WNDPROC window_proc> struct window_class<controls::tristate, window_proc> : super_window_class<_::sWC_BUTTON, controls::tristate, window_proc> {};

}
