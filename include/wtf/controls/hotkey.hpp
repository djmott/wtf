/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {

    namespace _ {

      TCHAR sHOTKEY_CLASS[] = HOTKEY_CLASS;

      template <typename _impl_t> using hotkey_impl = window_impl<_impl_t,
        policy::has_font,
        policy::has_text,
        wtf::policy::has_move
      >;

    }

    /** @class hotkey
    @ingroup Widgets
    @brief A hot key control is a window that enables the user to enter a combination of keystrokes to be used as a hot key.
    */
    struct hotkey : _::hotkey_impl<hotkey> {

      enum class modifiers : uint8_t{
        none = 0,
        shift = HOTKEYF_SHIFT,
        ctrl = HOTKEYF_CONTROL,
        alt = HOTKEYF_ALT,
        ext = HOTKEYF_EXT,
      };

      hotkey() : _::hotkey_impl<hotkey>() {
        wtf::_::init_common_controls<wtf::_::hotkey_classes>::get();
      }


      uint8_t keycode() const {
        auto lRet = LOBYTE(LOWORD(::SendMessage(*this, HKM_GETHOTKEY, 0, 0)));
        return static_cast<uint8_t>(lRet);
      }

      modifiers modifier() const {
        auto lRet = HIBYTE(LOWORD(::SendMessage(*this, HKM_GETHOTKEY, 0, 0)));
        return static_cast<modifiers>(lRet);
      }

      void keycode(uint8_t newval) {
        ::SendMessage(*this, WM_SETHOTKEY, MAKEWORD(newval, static_cast<uint8_t>(modifier())), 0);
      }

      void modifier(modifiers newval) {
        ::SendMessage(*this, WM_SETHOTKEY, MAKEWORD(keycode(), static_cast<uint8_t>(newval)), 0);
      }



    };

  }

  template <WNDPROC window_proc>
  struct window_class<controls::hotkey, window_proc> :
    super_window_class<controls::_::sHOTKEY_CLASS, controls::hotkey, window_proc> {};

}