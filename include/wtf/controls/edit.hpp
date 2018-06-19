/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {

    namespace _ {

#if WTF_USE_COMMON_CONTROLS
      TCHAR sWC_EDIT[] = WC_EDIT;
#else
      TCHAR sWC_EDIT[] = _T("EDIT");
#endif
    }

    /** @class edit
    @ingroup Widgets
    @brief An edit control is a rectangular control window typically used in a dialog box to permit the user to enter and edit text by typing on the keyboard.
    @tparam _multiline Indicates that multiple lines of text are permitted.
    */
    template <bool _multiline>
    struct edit :window_impl<edit<_multiline>,
      policy::has_font,
      policy::has_text,
      wtf::policy::has_move
    > {

      static constexpr DWORD Style = window::Style | (_multiline ? ES_MULTILINE : 0);

      void readonly(bool newval) {
        wtf::exception::throw_lasterr_if(::SendMessage(*this, EM_SETREADONLY, (newval ? TRUE : FALSE), 0), [](LRESULT l) { return 0 == l; });
      }

    };

  }
  
  template <bool _multiline, WNDPROC window_proc> 
  struct window_class<controls::edit<_multiline>, window_proc> :
    super_window_class<controls::_::sWC_EDIT, controls::edit<_multiline>, window_proc> {};

}