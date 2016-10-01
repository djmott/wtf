#pragma once
namespace wtf{
  namespace system_theme{
    struct label : window<label, has_cursor, has_background, has_move, has_show>{
      label(HWND hParent) : window(hParent){
      }
      virtual ~label() = default;
    };
  }
}