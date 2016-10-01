#pragma once

namespace wtf{

#define throw_lasterr_if( _test, _expression ) _throw_lasterr_if( _test, __FILE__, __LINE__, #_test, _expression )

  struct exception : std::runtime_error{
    using _super_t = std::runtime_error;

    template <typename _TestT, typename _ExprT>
    static _TestT _throw_lasterr_if(_TestT test, const char * sfile, int line, const char * sTest, _ExprT expr){
      if (expr(test)) throw exception(sfile, line, sTest, GetLastError());
      return test;
    }
  
    ~exception() = default;

//     exception(const char * sfile, int line, const char * code, Gdiplus::Status last_error) : _super_t(""), _file(sfile), _line(line), _code(code), _what(code){}

    exception(const char * sfile, int line, const char * code, DWORD last_error) : _super_t(""), _file(sfile), _line(line), _code(code), _what(code){
      LPSTR sBuff = nullptr;
      if (::FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, nullptr, last_error, 0, reinterpret_cast<LPSTR>(&sBuff), 0, nullptr) && sBuff){
        _what = sBuff;
      }
      if (sBuff) ::LocalFree(sBuff);
    }



    const char * file() const{ return _file.c_str(); }
    int line() const{ return _line; }
    const char * code() const{ return _code.c_str(); }
#if (__MINGW)
    virtual char const* what() const noexcept { return _what.c_str(); }    
#elif (_MSC_VER)
    virtual char const* what() const{ return _what.c_str(); }
#endif

  private:

    std::string _file;
    int _line;
    std::string _code;
    std::string _what;

  };
}