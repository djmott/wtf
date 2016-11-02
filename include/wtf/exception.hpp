/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  #define throw_lasterr_if(_test, _expression) _throw_lasterr_if( _test, __FILE__, __LINE__, #_test, _expression )

    struct exception : std::runtime_error{
      using __super_t = std::runtime_error;

      template<typename _TestT, typename _ExprT>
      static _TestT _throw_lasterr_if(_TestT test, const char *sfile, int line, const char *sTest, _ExprT expr){
        if (!expr(test)) return test; 
        throw exception(sfile, line, sTest, GetLastError());        
      }

      ~exception() = default;

      exception(const char *sfile, int line, const char *code, DWORD last_error) : __super_t(""), _file(sfile),
        _code(code),
        _what(code),
        _line(line)
      {
        LPSTR sBuff = nullptr;
        if (::FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, nullptr, last_error, 0,
            reinterpret_cast<LPSTR>(&sBuff), 0, nullptr) && sBuff){
          _what = sBuff;
        }
        if (sBuff) ::LocalFree(sBuff);
      }

      exception(const exception& src) 
        : runtime_error(src), _file(src._file), _code(src._code), _what(src._what), _line(src._line){}
      exception(exception&& src) 
        : runtime_error(std::move(src)), _file(std::move(src._file)), _code(std::move(src._code)), 
        _what(std::move(src._what)), _line(src._line){}

      exception& operator=(const exception& src){
        if (&src == this) return *this;
        runtime_error::operator=(src);
        _file = src._file;
        _code = src._code;
        _what = src._what;
        _line = src._line;
        return *this;
      }

      exception& operator=(exception&& src){
        if (&src == this) return *this;
        runtime_error::operator=(std::move(src));
        _file = std::move(src._file);
        _code = std::move(src._code);
        _what = std::move(src._what);
        _line = src._line;
        return *this;
      }

      const char *file() const{ return _file.c_str(); }

      int line() const{ return _line; }

      const char *code() const{ return _code.c_str(); }

    #if defined(__MINGW)
      virtual char const* what() const noexcept{ return _what.c_str(); }
    #elif defined(_MSC_VER)
      virtual char const* what() const{ return _what.c_str(); }
    #endif

    private:

      std::string _file;
      std::string _code;
      std::string _what;
      int _line;
    };


}
