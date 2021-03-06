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
    @ingroup Controls
    @image html button.png
    */
    struct button : DOXY_INHERIT_BUTTON_SUPER window_impl<button,
      policy::has_text,
      policy::has_font,
      policy::has_enable,
      policy::has_invalidate,
      policy::has_move,
      policy::has_style,
      messages::wm_command
    > {

      //! @brief Gets the ideal size of the control
      size ideal_size() const {
        size oRet;
        wtf::exception::throw_lasterr_if(::SendMessage(*this, BCM_GETIDEALSIZE, 0, reinterpret_cast<LPARAM>(&oRet)), [](LRESULT l) { return !l; });
        return oRet;
      }

    protected:
      template <typename, template <typename> typename...> friend struct window_impl;
#if WTF_USE_COMMON_CONTROLS
      static constexpr TCHAR sub_window_class_name[] = WC_BUTTON;
#else
      static constexpr TCHAR sub_window_class_name[] = _T("BUTTON");
#endif
      static constexpr TCHAR window_class_name[] = _T("wtf_button");
      template <WNDPROC wp> using window_class_type = super_window_class<window_class_name, sub_window_class_name, wp>;
    private:
      void add(window &) override {}

    };


    /** @class checkbox
    @brief A dual state (boolean) check box.
    @ingroup Controls
    @image html checkbox.png
    */
    struct checkbox : button {
      checkbox() {
        window::_style |= BS_AUTOCHECKBOX;
      }
    };


    /** @class radio_group
    @brief A radio_button that starts a group.
    @details Only a single radio_button in a radio_group may be selected at a time
    @ingroup Controls
    @image html radio_button.png
    */
    struct radio_group : button {
      radio_group() {
        window::_style |= (BS_AUTORADIOBUTTON | WS_GROUP);
      }
    };


    /** @class radio_button
    @brief An element in a radio_group. Only a single item in a radio_group can be selected at one time
    @ingroup Controls
    @image html radio_button.png
    */
    struct radio_button : button {
      radio_button() {
        window::_style |= BS_AUTORADIOBUTTON;
      }
    };

    /** @class tristate
    @brief A tri-state check box.
    @details A tristate is similar to a checkbox. It has an on-off state like a checkbox plus a third greyed state.
    @ingroup Controls
     @image html tristate.png
   */
    struct tristate : button {
      tristate() {
        window::_style |= BS_AUTO3STATE;
      }
    };

  }

}
