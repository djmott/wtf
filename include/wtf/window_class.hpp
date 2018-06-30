/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once
namespace wtf {


  template <const TCHAR * _class_name, WNDPROC window_proc>
  struct NOVTABLE window_class : WNDCLASSEX {

    static window_class& get() {
      static window_class _window_class_ex;
      return _window_class_ex;
    }

    int window_extra_offset() const { return 0; }

    WNDPROC default_window_proc() const noexcept { return DefWindowProc; }

    ~window_class() { UnregisterClass(_class_name, instance_handle()); }
    window_class(const window_class&) = default;
    window_class& operator=(const window_class& src) = default;
  protected:
    window_class() {
      memset(this, 0, sizeof(WNDCLASSEX));
      cbSize = sizeof(WNDCLASSEX);
      style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
      lpfnWndProc = window_proc;
      cbWndExtra = sizeof(void*);
      hInstance = instance_handle();
      lpszClassName = _class_name;
      exception::throw_lasterr_if(RegisterClassEx(this), [](ATOM x) noexcept { return 0 == x; });
    }

  };

  template <const TCHAR * _class_name, const TCHAR * _subclass_name, WNDPROC window_proc> 
  struct NOVTABLE super_window_class : WNDCLASSEX {
    
    static super_window_class& get() {
      static super_window_class _window_class_ex;
      return _window_class_ex;
    }

    WNDPROC default_window_proc() { return _original_proc; }

    int window_extra_offset() const { return _window_extra_offset; }

    ~super_window_class() { UnregisterClass(_class_name, instance_handle()); }
    super_window_class(const super_window_class&) = default;
    super_window_class& operator=(const super_window_class& src) = default;
  protected:
    super_window_class() {
      memset(this, 0, sizeof(WNDCLASSEX));
      cbSize = sizeof(WNDCLASSEX);
      exception::throw_lasterr_if(GetClassInfoEx(instance_handle(), _subclass_name, this), [](BOOL b) { return 0 == b; });
      style &= ~CS_GLOBALCLASS;
      _original_proc = lpfnWndProc;
      _window_extra_offset = cbWndExtra;
      cbWndExtra += sizeof(void*);
      lpfnWndProc = window_proc;
      hInstance = instance_handle();
      lpszClassName = _class_name;
      exception::throw_lasterr_if(RegisterClassEx(this), [](ATOM x) noexcept { return 0 == x; });
    }

  private:
    WNDPROC _original_proc;
    int _window_extra_offset = 0;
  };

}
