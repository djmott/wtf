#pragma once

namespace wtf{

  namespace _{
    template <WNDPROC _WP> using button_class = window_class_ex<_WP, 0, WS_CHILD|WS_VISIBLE, CS_GLOBALCLASS | CS_OWNDC | CS_PARENTDC, icons::null_icon, cursors::null_cursor, solid_brush<RGB(255,0,0)>, menus::null_menu, icons::null_icon>;
  }

  struct button : window<button, _::button_class, wm_mousemove>{
    virtual void OnMouseMove(buttons, int x, int y) override{
      OutputDebugStringA("MOUSE!!!\n");
    }
    button(HWND parent) : window(parent){}
  };
}