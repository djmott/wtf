/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#define DOXY_INHERIT_HOTKEY_SUPER \
  DOXY_INHERIT_WINDOW \
  DOXY_INHERIT_HAS_FONT \
  DOXY_INHERIT_HAS_TEXT \
  DOXY_INHERIT_HAS_MOVE

namespace wtf {
  namespace controls {


    /** @class hotkey
    @brief A hot key control enables the user to enter a combination of keystrokes to be used as a hot key.
    @ingroup Controls
    @image html hotkey.png
    */
    struct hotkey : DOXY_INHERIT_HOTKEY_SUPER window_impl<hotkey,
      policy::has_font,
      policy::has_text,
      policy::has_move
    > {

      enum class modifiers : uint8_t{
        none = 0,
        shift = HOTKEYF_SHIFT,
        ctrl = HOTKEYF_CONTROL,
        alt = HOTKEYF_ALT,
        ext = HOTKEYF_EXT,
      };

      //! @brief gets the keycode
      uint8_t keycode() const {
        auto lRet = LOBYTE(LOWORD(::SendMessage(*this, HKM_GETHOTKEY, 0, 0)));
        return static_cast<uint8_t>(lRet);
      }

      //! @brief sets the keycode
      void keycode(uint8_t newval) {
        ::SendMessage(*this, WM_SETHOTKEY, MAKEWORD(newval, static_cast<uint8_t>(modifier())), 0);
      }

      //! @brief gets the modifier
      modifiers modifier() const {
        auto lRet = HIBYTE(LOWORD(::SendMessage(*this, HKM_GETHOTKEY, 0, 0)));
        return static_cast<modifiers>(lRet);
      }

      //! @brief sets the modifier
      void modifier(modifiers newval) {
        ::SendMessage(*this, WM_SETHOTKEY, MAKEWORD(keycode(), static_cast<uint8_t>(newval)), 0);
      }
      
    };

  }

#if !DOXY_INVOKED

  namespace _ {
    TCHAR sHOTKEY_CLASS[] = HOTKEY_CLASS;
  }
  
  template <WNDPROC window_proc>
  struct window_class<controls::hotkey, window_proc> :
    super_window_class<_::sHOTKEY_CLASS, controls::hotkey, window_proc> {};

#endif
}