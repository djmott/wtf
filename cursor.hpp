#pragma once

namespace wtf{

  template <LPTSTR _ID>
  struct cursor{
    cursor() : _hcursor(LoadCursor(reinterpret_cast<HINSTANCE>(&__ImageBase), reinterpret_cast<LPCTSTR>(_ID))){}
    ~cursor(){ DestroyCursor(_hcursor); }
    HCURSOR native_handle() const{ return _hcursor; }
    HCURSOR operator()() const{ return _hcursor; }
    operator HCURSOR() const{ return _hcursor; }
  private:
    HCURSOR _hcursor;
  };

  namespace cursors{

    struct null_cursor{
      HCURSOR native_handle() const{ return nullptr; }
      HCURSOR operator()() const{ return nullptr; }
      operator HCURSOR() const{ return nullptr; }
    };

    using app_starting = cursor<IDC_APPSTARTING>;
    using arrow = cursor<IDC_ARROW>;
    using cross = cursor<IDC_CROSS>;
    using hand = cursor<IDC_HAND>;
    using help = cursor<IDC_HELP>;
    using ibeam = cursor<IDC_IBEAM>;
    using icon = cursor<IDC_ICON>;
    using no = cursor<IDC_NO>;
    using size = cursor<IDC_SIZE>;
    using size_all = cursor<IDC_SIZEALL>;
    using size_nesw = cursor<IDC_SIZENESW>;
    using size_ns = cursor<IDC_SIZENS>;
    using size_we = cursor<IDC_SIZEWE>;
    using up_arrow = cursor<IDC_UPARROW>;
    using wait = cursor<IDC_WAIT>;
  }

}