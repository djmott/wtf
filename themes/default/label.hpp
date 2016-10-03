#pragma once
namespace wtf{
  namespace default_theme{
    struct label : window<label, has_cursor, has_paint, has_size, has_show, has_text, has_border, has_click>{
      label(HWND hParent) : window(hParent, true){}
      virtual ~label() = default;

      label(const label&) = delete;
    };
  }
}
