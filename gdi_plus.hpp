#pragma once
namespace wtf{
  struct gdi_plus_initializer{
    static gdi_plus_initializer& get(){
      static gdi_plus_initializer _gdi_plus_initializer;
      return _gdi_plus_initializer;
    }
    ~gdi_plus_initializer(){ Gdiplus::GdiplusShutdown(_token); }
  private:
    gdi_plus_initializer(){
      Gdiplus::GdiplusStartupInput gdi_startup_input;
      auto iRet = Gdiplus::GdiplusStartup(&_token, &gdi_startup_input, nullptr);
      if (iRet != Gdiplus::Status::Ok) throw wtf::exception(__FILE__, __LINE__, "GdiplusStartup", iRet);
    }
    ULONG_PTR _token;
  };
}