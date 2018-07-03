/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  #define throw_lasterr_if(_test, _expression) _throw_lasterr_if( _test, __FILE__, __LINE__, #_test, _expression )

  #define throw_err( ... ) _throw_err( __VA_ARGS__ , __FILE__, __LINE__, # __VA_ARGS__ )

    struct exception : std::runtime_error{

      template<typename _TestT, typename _ExprT>
      static _TestT _throw_lasterr_if(_TestT test, const char *sfile, int line, const char *sTest, _ExprT expr) noexcept(false) {
        if (!expr(test)) return test; 
        throw exception(sfile, line, sTest, GetLastError());        
      }

      template<typename _TestT>
      static _TestT _throw_err(_TestT test, const char *sfile, int line, const char *sTest) noexcept(false) {
        if (NO_ERROR == test) return test;
        throw exception(sfile, line, sTest, test);
      }

      ~exception() = default;

      exception(const char *sfile, int line, const char *code, DWORD last_error) : std::runtime_error(""), _file(sfile),
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
#if _DEBUG
        tstring sMsg = _T("Exception ") + to_tstring(last_error) + _T(" at: ") + to_tstring(sfile) + _T("(") + to_tstring(line) + _T(")\n");
        sMsg += to_tstring(code) + _T("\n") + to_tstring(_what) + _T("\n");
        OutputDebugString(sMsg.c_str());
#endif
        assert(false);
      }

      exception(const exception& src) 
        : runtime_error(src), _file(src._file), _code(src._code), _what(src._what), _line(src._line){}
      exception(exception&& src) noexcept
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

      exception& operator=(exception&& src) noexcept {
        if (&src == this) return *this;
        runtime_error::operator=(std::move(src));
        _file = std::move(src._file);
        _code = std::move(src._code);
        _what = std::move(src._what);
        _line = src._line;
        return *this;
      }

      const char *file() const noexcept { return _file.c_str(); }

      int line() const noexcept { return _line; }

      const char *code() const noexcept { return _code.c_str(); }

    #if defined(__MINGW)
      char const* what() const noexcept{ return _what.c_str(); }
    #elif defined(_MSC_VER)
      char const* what() const override { return _what.c_str(); }
    #endif
    protected:
      exception(const char *sfile, int line, const char *code, const char * what) : std::runtime_error(""), _file(sfile), _line(line), _code(code), _what(what){}
    private:

      std::string _file;
      int _line;
      std::string _code;
      std::string _what;
    };


}
