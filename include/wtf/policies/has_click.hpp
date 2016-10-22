#pragma once

namespace wtf {
  namespace policy {
    /** has_click
    * produces click events
    */
    template<typename _SuperT, typename _ImplT>
    struct has_click : _SuperT {
      using mouse_msg_param = messages::mouse_msg_param;

    protected:

      virtual void wm_click(const mouse_msg_param&){}
      
      has_click(window<void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool &) {

        if (WM_MOUSEMOVE == umsg){
          auto oRect = rect<coord_frame::client>::get(hwnd);
          auto x = LOWORD(lparam);
          auto y = HIWORD(lparam);
          if (x < 0 || y < 0 || x > oRect.right || y > oRect.bottom){
            _Down = mouse_msg_param::buttons::unspecified;
          }
        }else if (WM_LBUTTONDOWN == umsg && mouse_msg_param::buttons::unspecified == _Down){
          _Down =  mouse_msg_param::buttons::left;
        } else if (WM_RBUTTONDOWN == umsg && mouse_msg_param::buttons::unspecified == _Down){
          _Down = mouse_msg_param::buttons::right;
        }else if (WM_MBUTTONDOWN==umsg && mouse_msg_param::buttons::unspecified == _Down){
          _Down = mouse_msg_param::buttons::middle;
        }else if (WM_LBUTTONUP == umsg && mouse_msg_param::buttons::left == _Down) {
          wm_click(mouse_msg_param(wparam, lparam, mouse_msg_param::buttons::left));
        } else if (WM_MBUTTONUP == umsg && mouse_msg_param::buttons::middle == _Down){
          wm_click(mouse_msg_param(wparam, lparam, mouse_msg_param::buttons::middle));
        } else if (WM_RBUTTONUP == umsg && mouse_msg_param::buttons::right == _Down){
          wm_click(mouse_msg_param(wparam, lparam, mouse_msg_param::buttons::right));
        }
        
        if (WM_LBUTTONUP == umsg || WM_RBUTTONUP == umsg || WM_MBUTTONUP == umsg){
          _Down = mouse_msg_param::buttons::unspecified;
        }


        return 0;
      }
    private:
       mouse_msg_param::buttons _Down = mouse_msg_param::buttons::unspecified;
    };
  }
}
