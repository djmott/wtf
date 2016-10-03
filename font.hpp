#pragma once

namespace wtf{
  struct font : LOGFONT{
    using map = std::map<tstring, font>;

    font(const font& src){ memcpy(this, &src, sizeof(LOGFONT)); }
    font(const LOGFONT& src){ memcpy(this, &src, sizeof(LOGFONT)); }

    /*
        static map get_fonts(device_context& oDC, const tstring& sFaceName=_T("")){
          map oRet;
          LOGFONT lf;
          memset(&lf, 0, sizeof(LOGFONT));
          enum_info oEnumInfo(oDC, oRet);
          lf.lfCharSet = DEFAULT_CHARSET;
          _tcscpy_s(lf.lfFaceName, LF_FACESIZE, sFaceName.c_str());
          ::EnumFontFamiliesEx(oDC, &lf, &font_enum_proc, reinterpret_cast<LPARAM>(&oEnumInfo), 0);
          return oRet;
        }
    */

    struct handle : std::unique_ptr<HFONT__, void(*)(HFONT)>{
      operator HFONT() const{ return get(); }

      handle(handle&& src) : unique_ptr(std::move(src)){}

      handle& operator=(handle&& src){
        unique_ptr::swap(src);
        return *this;
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

    protected:
      friend struct font;
      template <typename ... _ArgTs> handle(_ArgTs&&...oArgs) : unique_ptr(std::forward<_ArgTs>(oArgs)...){}
    };

    handle open() const{
      return handle(wtf::exception::throw_lasterr_if(::CreateFontIndirect(this), [](HFONT f){return !f; }), [](HFONT f){ ::DeleteObject(f); });
    }

    /*
      private:

        device_context& _dc;

        struct enum_info{
          enum_info(device_context& oDC, map& oMap) : _DC(oDC), _Map(oMap){}
          device_context& _DC;
          map& _Map;
        };

        static int CALLBACK font_enum_proc(const LOGFONT *lplf, const TEXTMETRIC *lptm, DWORD dwType, LPARAM lpData){
          auto pEnumInfo = reinterpret_cast<enum_info*>(lpData);
          pEnumInfo->_Map.insert(std::make_pair(tstring(lplf->lfFaceName, LF_FACESIZE), font(lplf, pEnumInfo->_DC)));
          return 1;
        }*/
  };
}