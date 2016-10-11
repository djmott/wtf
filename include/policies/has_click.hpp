#pragma once

namespace wtf {
  namespace policy {
    /** has_click
    * produces click events
    */
    template<typename _SuperT, typename _ImplT>
    struct has_click : _SuperT {

      callback<void()> OnClick;

    protected:

      virtual void ClickEvent(const point::client_coords&){ OnClick(); }

      virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM , LPARAM lparam, bool &) override {
        if (WM_LBUTTONDOWN == umsg) {
          _Down = true;
        }
        if (WM_MOUSEMOVE == umsg && _Down) {
          auto oRect = rect::client_coord::get(hwnd);
          auto x = LOWORD(lparam);
          auto y = HIWORD(lparam);
          if (x < 0 || y < 0 || x > oRect.right || y > oRect.bottom) {
            _Down = false;
          }
        }
        if (WM_LBUTTONUP == umsg && _Down) {
          _Down = false;
          ClickEvent(point::client_coords(LOWORD(lparam), HIWORD(lparam)));
        }
        return 0;
      }

      bool _Down = false;
    };
  }
}
