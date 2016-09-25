#pragma once

namespace wtf{
  namespace default_theme{

    namespace _{
      template <WNDPROC _proc> using button_class = window_class_ex<_proc, 0, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, CS_GLOBALCLASS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW, icons::application, cursors::arrow, create_window_system_brush<system_colors::button_face>, menus::null_menu, icons::application>;
    }

    struct button : wtf::window<button, _::button_class, wm_lbuttondown, wm_mousemove, wm_lbuttonup, wm_paint>{

      virtual void OnPaint(const RECT& area, Gdiplus::Graphics& oGraphics){
        oGraphics.DrawLine(&_HighlightPen, 0, 0, area.right, 0);
        oGraphics.DrawLine(&_HighlightPen, 0, 0, 0, area.bottom);
        oGraphics.DrawLine(&_ShadowPen, area.right - 1, 0, area.right - 1, area.bottom);
        oGraphics.DrawLine(&_ShadowPen, 0, area.bottom - 1, area.right - 1, area.bottom - 1);
      }


      button(HWND hParent) : window(hParent), _TextColor(GetSysColor(COLOR_BTNTEXT)), _Highlight(GetSysColor(COLOR_BTNHIGHLIGHT)), _Shadow(GetSysColor(COLOR_BTNSHADOW)), _HighlightPen(_Highlight), _ShadowPen(_Shadow), _TextBrush(_TextColor){}

    private:
      Gdiplus::Color _TextColor;
      Gdiplus::Color _Highlight;
      Gdiplus::Color _Shadow;
      Gdiplus::Pen _HighlightPen;
      Gdiplus::Pen _ShadowPen;
      Gdiplus::SolidBrush _TextBrush;

    };

  }
}