#pragma once
namespace wtf{
  namespace default_theme{
    struct label : window<label, has_text_metrics, has_font, has_cursor, has_background, has_size, has_show, has_text, has_border>{

      label(HWND hParent) : window(hParent, true){  }
      virtual ~label() = default;

      label(const label&) = delete;

    };
  }
}