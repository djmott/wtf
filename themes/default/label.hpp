#pragma once
namespace wtf{
  namespace default_theme{
    struct label : window<label, wm_paint, has_cursor, has_background, has_move, has_show>{
      static const DWORD Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS;
      label(HWND parent) : window(parent){ show(); }
      virtual ~label() = default;
      virtual void OnPaint(const RECT& area, wtf::device_context& oCtx) override{
        solid_pen<1, static_rgb<255, 0, 0>> oPen;
        oCtx.line(oPen, 0, 0, 100, 100);
      }
    };
  }
}