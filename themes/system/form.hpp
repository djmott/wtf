#pragma once

namespace wtf{
  namespace system_theme{

    struct form : window<form, wm_close, wm_move, wm_moving, wm_mousemove, has_cursor, has_titlebar, has_show, has_move, has_close, has_background>{
      form(HWND hParent = nullptr) : window(){
      }
    };
  }
}