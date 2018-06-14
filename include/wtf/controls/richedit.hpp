/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {

    namespace _ {

      TCHAR sMSFTEDIT_CLASS[] = MSFTEDIT_CLASS;

      template <typename _impl_t> using richedit_impl = window_impl<_impl_t,
        policy::has_font,
        policy::has_text,
        wtf::policy::has_move
      >;

    }

    template <bool _multiline=true>
    struct richedit : _::richedit_impl<richedit> {

      static constexpr DWORD Style = window::Style |
        (_multiline?ES_MULTILINE:0)
      richedit() : _::richedit_impl<richedit>() {
        wtf::exception::throw_lasterr_if(::LoadLibrary(_T("msftedit.dll")), [](HMODULE h) { return nullptr == h; });
      }

    protected:
      void handle_msg(wtf::window_message& msg) override {
        if (WM_COMMAND != msg.umsg) return;
        //this control must not forward parent redirected WM_COMMAND messages
        msg.bhandled = (msg.hwnd != _handle);
        if (!msg.bhandled) DebugBreak();
      }

    };

  }

  template <WNDPROC window_proc>
  struct window_class<controls::richedit, window_proc> :
    super_window_class<controls::_::sMSFTEDIT_CLASS, controls::richedit, window_proc> {};

}