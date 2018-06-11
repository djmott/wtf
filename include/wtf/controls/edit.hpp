/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {

    namespace _ {
      template <typename _impl_t> using edit_impl = window_impl<_impl_t,
        policy::has_font,
        policy::has_text,
        wtf::policy::has_move
      >;
    }

    struct edit : _::edit_impl<edit> {
      edit(window * parent) : _::edit_impl<edit>(parent){}
    };

  }


  template <WNDPROC window_proc> struct window_class<controls::edit, window_proc> {   
      constexpr LPCTSTR name() const noexcept { return WC_EDIT; }  
      static window_class& get() {      
        static window_class _window_class_ex;  
        return _window_class_ex;  
    }
  };

}