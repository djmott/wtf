/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once
namespace wtf {


  template <typename _impl_t, WNDPROC window_proc>
  struct NOVTABLE window_class : WNDCLASSEX {

    static window_class& get() {
      static window_class _window_class_ex;
      return _window_class_ex;
    }

    LPCTSTR name() const noexcept { return _class_name.c_str(); }

    int window_extra_offset() const { return 0; }

    WNDPROC default_window_proc() const noexcept { return DefWindowProc; }

    ~window_class() { UnregisterClass(_class_name.c_str(), instance_handle()); }
    window_class& operator=(const window_class& src) {
      _class_name = src._class_name;
      return *this;
    }
    window_class(const window_class& src) noexcept : _class_name(src._class_name) {}
    window_class(window_class&& src) noexcept : _class_name(std::move(src._class_name)) {}
    window_class& operator=(window_class&& src) noexcept {
      std::swap(_class_name, src._class_name);
      return *this;
    }
  protected:
    window_class() {
      memset(this, 0, sizeof(WNDCLASSEX));
      _class_name = _T("wtf:");
#if defined(UNICODE)
      _class_name += std::to_wstring(typeid(_impl_t).hash_code());
#else
      _class_name += std::to_string(typeid(_impl_t).hash_code());
#endif
      cbSize = sizeof(WNDCLASSEX);
      style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
      lpfnWndProc = window_proc;
      cbWndExtra = sizeof(_impl_t*);
      hInstance = instance_handle();
      lpszClassName = name();
      exception::throw_lasterr_if(RegisterClassEx(this), [](ATOM x) noexcept { return 0 == x; });
    }

    tstring _class_name;
  };

  template <const TCHAR * _original_class_name, typename _impl_t, WNDPROC window_proc> 
  struct NOVTABLE super_window_class : WNDCLASSEX {
    
    static super_window_class& get() {
      static super_window_class _window_class_ex;
      return _window_class_ex;
    }

    LPCTSTR name() const noexcept { return _class_name.c_str(); }

    WNDPROC default_window_proc() { return _original_proc; }

    int window_extra_offset() const { return _window_extra_offset; }

    super_window_class() {
      memset(this, 0, sizeof(WNDCLASSEX));
      cbSize = sizeof(WNDCLASSEX);
      exception::throw_lasterr_if(GetClassInfoEx(instance_handle(), _original_class_name, this), [](BOOL b) { return 0 == b; });
      style &= ~CS_GLOBALCLASS;
      _original_proc = lpfnWndProc;
      _window_extra_offset = cbWndExtra;
      cbWndExtra += sizeof(_impl_t*);
      _class_name = _T("wtf:");
#if defined(UNICODE)
      _class_name += std::to_wstring(typeid(_impl_t).hash_code());
#else
      _class_name += std::to_string(typeid(_impl_t).hash_code());
#endif
      lpfnWndProc = window_proc;
      hInstance = instance_handle();
      lpszClassName = name();
      exception::throw_lasterr_if(RegisterClassEx(this), [](ATOM x) noexcept { return 0 == x; });

    }

  private:
    WNDPROC _original_proc;
    tstring _class_name;
    int _window_extra_offset = 0;
  };

}
