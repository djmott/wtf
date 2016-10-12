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

      virtual void ClickEvent(const mouse_event&){ OnClick(); }

      virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool &) override {

        if (WM_MOUSEMOVE == umsg){
          auto oRect = rect::client_coord::get(hwnd);
          auto x = LOWORD(lparam);
          auto y = HIWORD(lparam);
          if (x < 0 || y < 0 || x > oRect.right || y > oRect.bottom){
            _Down = mouse_event::buttons::unspecified;
          }
        }else if (WM_LBUTTONDOWN == umsg && mouse_event::buttons::unspecified == _Down){
          _Down =  mouse_event::buttons::left;
        } else if (WM_RBUTTONDOWN == umsg && mouse_event::buttons::unspecified == _Down){
          _Down = mouse_event::buttons::right;
        }else if (WM_MBUTTONDOWN==umsg && mouse_event::buttons::unspecified == _Down){
          _Down = mouse_event::buttons::middle;
        }else if (WM_LBUTTONUP == umsg && mouse_event::buttons::left == _Down) {
          mouse_event oEvent(mouse_event::buttons::left, static_cast<mouse_event::key_states>(wparam),
                             static_cast<int16_t>(LOWORD(lparam)), static_cast<int16_t>(HIWORD(lparam)));
          ClickEvent(oEvent);
        } else if (WM_RBUTTONUP == umsg && mouse_event::buttons::right == _Down){
          mouse_event oEvent(mouse_event::buttons::right, static_cast<mouse_event::key_states>(wparam),
                             static_cast<int16_t>(LOWORD(lparam)), static_cast<int16_t>(HIWORD(lparam)));
          ClickEvent(oEvent);
        } else if (WM_MBUTTONUP == umsg && mouse_event::buttons::middle == _Down){
          mouse_event oEvent(mouse_event::buttons::middle, static_cast<mouse_event::key_states>(wparam),
                             static_cast<int16_t>(LOWORD(lparam)), static_cast<int16_t>(HIWORD(lparam)));
          ClickEvent(oEvent);
        } 
        
        if (WM_LBUTTONUP == umsg || WM_RBUTTONUP == umsg || WM_MBUTTONUP == umsg){
          _Down = mouse_event::buttons::unspecified;
        }


        return 0;
      }
    private:
       mouse_event::buttons _Down = mouse_event::buttons::unspecified;
    };
  }
}