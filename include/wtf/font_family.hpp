/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once
namespace  wtf {


    struct font_family {
      using ptr = std::shared_ptr<font_family>;
      using vector = std::vector<font_family>;

      static vector get(HDC hdc) {
        vector oRet;
        EnumFontFamilies(hdc, nullptr, &enum_callback, &oRet)
      }

    private:
      static int CALLBACK
      EnumFontFamProc(ENUMLOGFONT
      *lpelf,
      NEWTEXTMETRIC *lpntm, DWORD
      FontType,
      LPARAM lParam
      ){
        auto oVector = reinterpret_cast<vector *>(lParam);
      }
    };
  }

