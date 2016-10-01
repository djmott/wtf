#pragma once

namespace wtf{

  struct cursor{

    virtual ~cursor(){ if (_hcursor) DestroyCursor(_hcursor); }

    cursor(HCURSOR hCursor) : _hcursor(hCursor){}
    cursor(const cursor& src) : _hcursor(CopyCursor(src._hcursor)){}
    cursor(cursor&& src) : _hcursor(std::move(src._hcursor)){}

    virtual HCURSOR operator*() const { return _hcursor; }
    virtual operator HCURSOR() const { return _hcursor; }
  protected:
    HCURSOR _hcursor;
  };

  namespace cursors{
        
    template <int _ID>
    struct system_cursor : cursor{
      virtual ~system_cursor() = default;
      system_cursor() : cursor(wtf::exception::throw_lasterr_if(LoadCursor(nullptr, MAKEINTRESOURCE(_ID)), [](HCURSOR h){return !h; })){}
      system_cursor(const system_cursor& src) : cursor(src){}
      system_cursor(system_cursor&& src) : cursor(std::move(src)){}
      
    };


    struct null_cursor : cursor{
      virtual ~null_cursor() override{}
      null_cursor() : cursor(nullptr){}

      virtual HCURSOR operator*() const override{ return nullptr; }
      virtual operator HCURSOR() const override{ return nullptr; }
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