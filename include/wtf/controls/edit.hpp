/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#define DOXY_INHERIT_EDIT_SUPER \
  DOXY_INHERIT_WINDOW \
  DOXY_INHERIT_HAS_FONT \
  DOXY_INHERIT_HAS_TEXT \
  DOXY_INHERIT_HAS_SIZE \
  DOXY_INHERIT_HAS_BORDER \
  DOXY_INHERIT_HAS_FOCUS \
  DOXY_INHERIT_HAS_MOVE \
  DOXY_INHERIT_HAS_STYLE

namespace wtf {
  
  namespace controls {

    /** @class edit
    @brief An editable box of text
    @ingroup Widgets
    */
    struct edit : DOXY_INHERIT_EDIT_SUPER window_impl<edit,
      policy::has_font,
      policy::has_text,
      policy::has_size,
      policy::has_border,
      policy::has_focus,
      policy::has_move,
      policy::has_style
    > {

      static const uint16_t default_input_limit = 32767;
    
      //! @brief makes the edit box read-only so user input is ignored
      void readonly(bool newval) {
        wtf::exception::throw_lasterr_if(::SendMessage(*this, EM_SETREADONLY, (newval ? TRUE : FALSE), 0), [](LRESULT l) { return 0 == l; });
      }

      //! @brief Gets the displayed character.
      //! @returns zero if no display masking is enabled or the display mask character
      TCHAR password_char() const { return ::SendMessage(*this, EM_GETPASSWORDCHAR, 0, 0); }

      //! @brief Sets or resets the display character.
      //! @param[in] newval the character that will be displayed to the user. Set to zero to disable masking
      void password_char(TCHAR newval) { ::SendMessage(*this, EM_SETPASSWORDCHAR, newval, 0); }

      //! @brief gets the lower-case conversion
      //! @returns true if the edit box converts all upper case input to lower case or false if the conversion is not enabled
      bool lower_case() const { return get_style<ES_LOWERCASE>(); }
      //! @brief sets lower-case conversion
      //! @param[in] newval set to true to enable lower-case conversion of user input or false to disable
      void lower_case(bool newval) { 
        set_style<ES_UPPERCASE>(false);
        set_style<ES_LOWERCASE>(newval);
      }
    
      //! @brief gets the upper-case conversion
      //! @returns true if the edit box converts all lower case input to upper case or false if the conversion is not enabled
      bool upper_case() const { return get_style<ES_UPPERCASE>(); }
      //! @brief sets upper-case conversion
      //! @param[in] newval set to true to enable upper-case conversion of user input or false to disable
      void upper_case(bool newval) {
        set_style<ES_LOWERCASE>(false);
        set_style<ES_UPPERCASE>(newval);
      }

      //! @brief gets the auto-hscroll setting
      //! @details Automatically scrolls text to the right by 10 characters when the user types a character at the end of the line
      bool auto_hscroll() const { return get_style<ES_AUTOHSCROLL>(); }
      //! @brief Enables/disables auto horizontal scrolling
      //! @param[in] newval true to enable or false to disable
      //! @details @copydetails wtf::controls::edit::auto_hscroll() const
      void auto_hscroll(bool newval) { set_style<ES_AUTOHSCROLL>(newval); }

      //! @brief gets the text limit
      uint16_t text_limit() const { return static_cast<uint16_t>(::SendMessage(*this, EM_GETLIMITTEXT, 0, 0)); }
      //! @brief sets the text limit
      void text_limit(uint16_t newval) { ::SendMessage(*this, EM_LIMITTEXT, newval, 0); }

    };

    /** @class multiline_edit
    @brief An editable box of text
    @ingroup Widgets
    */
    struct multiline_edit : edit {
      static constexpr DWORD Style = window::Style | ES_MULTILINE;
    };
    
  }

#if !DOXY_INVOKED

  namespace _ {
#if WTF_USE_COMMON_CONTROLS
    TCHAR sWC_EDIT[] = WC_EDIT;
#else
    TCHAR sWC_EDIT[] = _T("EDIT");
#endif
  }

  template <WNDPROC window_proc>
  struct window_class<controls::edit, window_proc> :
    super_window_class<_::sWC_EDIT, controls::edit, window_proc> {};

  template <WNDPROC window_proc>
  struct window_class<controls::multiline_edit, window_proc> :
    super_window_class<_::sWC_EDIT, controls::multiline_edit, window_proc> {};

#endif

}