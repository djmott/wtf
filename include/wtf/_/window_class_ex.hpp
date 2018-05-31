/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once
namespace wtf{
  namespace _{

    template <typename _impl_t, WNDPROC window_proc>
    struct window_class_ex : WNDCLASSEX{

      static window_class_ex& get(){
        static window_class_ex _window_class_ex;
        return _window_class_ex;
      }

      LPCTSTR name() const noexcept { return _class_name.c_str(); }

      ~window_class_ex(){ UnregisterClass(_class_name.c_str(), instance_handle()); }
      window_class_ex& operator=(const window_class_ex& src) {
        _class_name = src._class_name;
        return *this;
      }
      window_class_ex(const window_class_ex& src) noexcept : _class_name(src._class_name) {}
      window_class_ex(window_class_ex&& src) noexcept : _class_name(std::move(src._class_name)) {}
      window_class_ex& operator=(window_class_ex&& src) noexcept {
        std::swap(_class_name, src._class_name);
        return *this;
      }
    private:
      window_class_ex()  {
        std::basic_string<TCHAR> asdf;
        memset(this, 0, sizeof(WNDCLASSEX));
        _class_name = _T("wtf:");
#if defined(UNICODE)
          _class_name += std::to_wstring(typeid(_impl_t).hash_code());
#else
          _class_name += std::to_string(typeid(_impl_t).hash_code());
#endif
        cbSize = sizeof(WNDCLASSEX);
        style = CS_OWNDC | CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
        lpfnWndProc = window_proc;
        cbClsExtra = sizeof(void*);
        hInstance = instance_handle();
        lpszClassName = name();
        exception::throw_lasterr_if(RegisterClassEx(this), [](ATOM x) noexcept { return 0 == x; });
      }

      tstring _class_name;
    };

  }
}