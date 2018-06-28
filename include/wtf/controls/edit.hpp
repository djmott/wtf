/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#define DOXY_INHERIT_EDIT_SUPER \
  DOXY_INHERIT_HAS_FONT \
  DOXY_INHERIT_HAS_TEXT \
  DOXY_INHERIT_HAS_SIZE \
  DOXY_INHERIT_HAS_BORDER \
  DOXY_INHERIT_HAS_FOCUS \
  DOXY_INHERIT_HAS_MOVE 

namespace wtf {
  namespace policy {


    template <typename _impl_t>
    struct edit_super_t : DOXY_INHERIT_EDIT_SUPER window_impl<_impl_t,
      policy::has_font,
      policy::has_text,
      policy::has_size,
      policy::has_border,
      policy::has_focus,
      policy::has_move
    > {


      void readonly(bool newval) {
        wtf::exception::throw_lasterr_if(::SendMessage(*this, EM_SETREADONLY, (newval ? TRUE : FALSE), 0), [](LRESULT l) { return 0 == l; });
      }

      TCHAR password_char() const { return ::InSendMessage(*this, EM_GETPASSWORDCHAR, 0, 0); }
      void password_char(TCHAR newval) { ::SendMessage(*this, EM_SETPASSWORDCHAR, newval, 0); }

    };

  }
  
  namespace controls {

    /** @class edit
    @brief An editable box of text
    @ingroup Widgets
    */
    struct edit : policy::edit_super_t<edit> {};

    /** @class multiline_edit
    @brief An editable box of text
    @ingroup Widgets
    */
    struct multiline_edit : policy::edit_super_t<multiline_edit> {
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