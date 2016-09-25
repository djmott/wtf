#pragma once

namespace wtf{

  template <int _ID>
  struct system_icon{
    system_icon() : _hicon(wtf::exception::throw_lasterr_if(LoadIcon(nullptr, MAKEINTRESOURCE(_ID)), [](HICON h){ return !h; })){}
    ~system_icon(){ DestroyIcon(_hicon); }
    HICON native_handle() const { return _hicon; }
    HICON operator()() const { return _hicon; }
    operator HICON() const { return _hicon; }
    
  private:
    HICON _hicon;
  };


  namespace icons{

    using application = system_icon<32512>;
    using hand = system_icon<32513>;
    using question = system_icon<32514>;
    using exclamation = system_icon<32515>;
    using asterisk = system_icon<32516>;
    using winlogo = system_icon<32517>;
    using shield = system_icon<32518>;

    struct null_icon{
      HICON native_handle() const{ return nullptr; }
      HICON operator()() const{ return nullptr; }
      operator HICON() const{ return nullptr; }
    };
  }

}