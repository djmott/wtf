#pragma once
namespace wtf{
  struct window_message{
    HWND hwnd;
    UINT umsg;
    WPARAM wparam;
    LPARAM lparam;
    bool bhandled;
    LRESULT lresult;
  };
}