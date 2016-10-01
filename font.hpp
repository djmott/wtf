#pragma once

namespace wtf{
  struct font : LOGFONT{
  
    using vector = std::vector<font>;

    font(const font& src){ memcpy(this, &src, sizeof(LOGFONT)); }
    font(const LOGFONT* src){ memcpy(this, src, sizeof(LOGFONT)); }


    static vector get_fonts(HDC hdc){
      vector oRet;
      ::EnumFonts(hdc, nullptr, &font_enum_proc, reinterpret_cast<LPARAM>(&oRet));
      return oRet;
    }

    struct handle : std::unique_ptr<HFONT__, void(*)(HFONT)>{
      operator HFONT() const{ return get(); }

      handle(handle&& src) : unique_ptr(std::move(src)){}

      handle& operator=(handle&& src){
        unique_ptr::swap(std::move(src));
        return *this;
      }

    protected:
      friend struct font;
      template <typename ... _ArgTs> handle(_ArgTs&&...oArgs) : unique_ptr(std::forward<_ArgTs>(oArgs)...){}

    };

    handle open() const{
      return handle(wtf::exception::throw_lasterr_if(::CreateFontIndirect(this), [](HFONT f){return !f; }), [](HFONT f){ ::DeleteObject(f); });
    }

    static handle ansi_fixed(){
      return handle(wtf::exception::throw_lasterr_if((HFONT)::GetStockObject(ANSI_FIXED_FONT), [](HFONT f){return !f; }), [](HFONT){});
    }

    static handle ansi_variable(){
      return handle(wtf::exception::throw_lasterr_if((HFONT)::GetStockObject(ANSI_VAR_FONT), [](HFONT f){return !f; }), [](HFONT){});
    }

    static handle device_default(){
      return handle(wtf::exception::throw_lasterr_if((HFONT)::GetStockObject(DEVICE_DEFAULT_FONT), [](HFONT f){return !f; }), [](HFONT){});
    }
    static handle gui_default(){
      return handle(wtf::exception::throw_lasterr_if((HFONT)::GetStockObject(DEFAULT_GUI_FONT), [](HFONT f){return !f; }), [](HFONT){});
    }

    static handle oem_fixed(){
      return handle(wtf::exception::throw_lasterr_if((HFONT)::GetStockObject(OEM_FIXED_FONT), [](HFONT f){return !f; }), [](HFONT){});
    }

    static handle system(){
      return handle(wtf::exception::throw_lasterr_if((HFONT)::GetStockObject(SYSTEM_FONT), [](HFONT f){return !f; }), [](HFONT){});
    }

    static handle system_fixed(){
      return handle(wtf::exception::throw_lasterr_if((HFONT)::GetStockObject(SYSTEM_FIXED_FONT), [](HFONT f){return !f; }), [](HFONT){});
    }

  private:
    static int CALLBACK font_enum_proc(CONST LOGFONT *lplf, CONST TEXTMETRIC *lptm, DWORD dwType, LPARAM lpData){
      auto oVector = reinterpret_cast<vector*>(lpData);
      oVector->emplace_back(lplf);
      return 1;
    }
  };
}