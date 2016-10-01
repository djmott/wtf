#pragma once


struct font_family{
  using ptr = std::shared_ptr<font_family>;
  using vector = std::vector<font_family>;

  static vector get(HDC hdc){
    vector oRet;
    EnumFontFamilies(hdc, nullptr, &enum_callback, &oRet)
  }

private:
  static int CALLBACK EnumFontFamProc(ENUMLOGFONT *lpelf, NEWTEXTMETRIC *lpntm, DWORD FontType, LPARAM lParam){
    auto oVector = reinterpret_cast<vector*>(lParam);

  }
};