#pragma once

namespace wtf{
  namespace messages{

    template<typename _SuperT, typename _ImplT>
    struct wm_dblclick : _SuperT{

    protected:

      virtual LRESULT on_wm_dblclick(const mouse_msg_param&, bool&) = 0{ return 0; }

      wm_dblclick(window<void,void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool & bHandled) {
        if (WM_LBUTTONDBLCLK == umsg){
          return on_wm_dblclick(mouse_msg_param(wparam, lparam, mouse_msg_param::buttons::left), bHandled);
        } else if (WM_MBUTTONDBLCLK == umsg){
          return on_wm_dblclick(mouse_msg_param(wparam, lparam, mouse_msg_param::buttons::middle), bHandled);
        } else if (WM_RBUTTONDBLCLK == umsg){
          return on_wm_dblclick(mouse_msg_param(wparam, lparam, mouse_msg_param::buttons::right), bHandled);
        }
        return 0;
      }
    };
  }
}
