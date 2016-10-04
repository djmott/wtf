#pragma once

namespace wtf {
  namespace policy {
    /** has_click
    * produces click and double click events
    */
    template<typename _SuperT>
    struct has_click : _SuperT {
      virtual ~has_click() = default;

      wtf::callback<void( const point
      &)>
      ClickEvent;

    protected:
      virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool &bhandled) override {
        if (WM_LBUTTONDOWN == umsg) {
          _Down = true;
          SetCapture(hwnd);
        }
        if (WM_MOUSEMOVE == umsg && _Down) {
          auto oRect = rect::get_client_rect(hwnd);
          auto x = LOWORD(lparam);
          auto y = HIWORD(lparam);
          if (x < 0 || y < 0 || x > oRect.right || y > oRect.bottom) {
            _Down = false;
            ReleaseCapture();
          }
        }
        if (WM_LBUTTONUP == umsg && _Down) {
          ReleaseCapture();
          _Down = false;
          ClickEvent(point(LOWORD(lparam), HIWORD(lparam)));
        }
        return 0;
      }

      bool _Down = false;
    };
  }
}
