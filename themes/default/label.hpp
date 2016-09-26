#pragma once
namespace wtf{
  namespace default_theme{
    struct label : window<label, 0, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, has_cursor, has_background, has_move, has_show, has_border>{
      label(HWND parent) : window(parent){ show(); }
      virtual ~label() = default;
    };
  }
}