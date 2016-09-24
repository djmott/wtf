#pragma once


namespace wtf{
  namespace _{
    template <WNDPROC _WP> using form_class = window_class_ex<_WP, WS_EX_WINDOWEDGE, WS_CLIPCHILDREN|WS_TILEDWINDOW, CS_GLOBALCLASS | CS_OWNDC | CS_PARENTDC, icons::null_icon, cursors::arrow, brushes::button_face, menus::null_menu, icons::null_icon>;
  }

  struct form : window<form, _::form_class, wm_move, wm_size, wm_moving>{
  
    form() : window(){}

    virtual void OnMoved(int x, int y) override{
      OutputDebugStringA("Window Moved!\n");
    }
    virtual void OnResized(type, int width, int height){
      OutputDebugStringA("Window Resized!\n");
    }
    virtual void OnMoving(LPRECT coords){
      OutputDebugStringA("Moving...\n");
    }


    int run(HWND Parent = nullptr){
      message oMsg;
      while (oMsg.get()){
        oMsg.translate();
        oMsg.dispatch();
      }
      return 0;
    }
  
  };

}