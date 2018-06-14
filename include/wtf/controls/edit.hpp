/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {

    namespace _ {

      TCHAR sWC_EDIT[] = WC_EDIT;

      template <typename _impl_t> using edit_impl = window_impl<_impl_t,
        policy::has_font,
        policy::has_text,
        wtf::policy::has_move
      >;

    }

    template <bool _multiline>
    struct edit : _::edit_impl<edit<_multiline>> {

      static constexpr DWORD Style = window::Style | (_multiline ? ES_MULTILINE : 0);
      edit() : _::edit_impl<edit<_multiline>>(){
        wtf::_::init_common_controls<wtf::_::standard_classes>::get();
      }

      void readonly(bool newval) {
        wtf::exception::throw_lasterr_if(::SendMessage(*this, EM_SETREADONLY, (newval ? TRUE : FALSE), 0), [](LRESULT l) { return 0 == l; });
      }

    };

  }
  
  template <bool _multiline, WNDPROC window_proc> 
  struct window_class<controls::edit<_multiline>, window_proc> :
    super_window_class<controls::_::sWC_EDIT, controls::edit<_multiline>, window_proc> {};

}