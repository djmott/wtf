#pragma once

namespace wtf{
  struct icon{};


  namespace icons{
    struct null_icon{
      HICON native_handle() const{ return nullptr; }
      HICON operator()() const{ return nullptr; }
      operator HICON() const{ return nullptr; }
    };
  }

}