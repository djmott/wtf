#pragma once

namespace wtf{

  template <int _ID>
  struct system_cursor{
    system_cursor() : _hcursor(wtf::exception::throw_lasterr_if(LoadCursor(nullptr, MAKEINTRESOURCE(_ID)), [](HCURSOR h){return !h; })){}
    ~system_cursor(){ DestroyCursor(_hcursor); }
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

    using app_starting = system_cursor<32650>;
    using arrow = system_cursor<32512>;
    using cross = system_cursor<32515>;
    using hand = system_cursor<32649>;
    using help = system_cursor<32651>;
    using ibeam = system_cursor<32513>;
    using icon = system_cursor<32641>;
    using no = system_cursor<32648>;
    using size = system_cursor<32640>;
    using size_all = system_cursor<32646>;
    using size_nesw = system_cursor<32643>;
    using size_ns = system_cursor<32645>;
    using size_we = system_cursor<32644>;
    using up_arrow = system_cursor<32516>;
    using wait = system_cursor<32514>;
  }

}