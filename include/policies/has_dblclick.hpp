#pragma once

namespace wtf{
  namespace policy{
    /** has_dblclick
    * produces double click events
    */
    template<typename _SuperT, typename _ImplT>
    struct has_dblclick : _SuperT{

      callback<void()> OnDblClick;

    protected:

      virtual void DblClickEvent(const point::client_coords&){ OnDblClick(); }

      virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM, LPARAM lparam, bool &) override{
        if (WM_LBUTTONDBLCLK == umsg){
          DblClickEvent(point::client_coords(LOWORD(lparam), HIWORD(lparam)));
        }
        return 0;
      }

      bool _Down = false;
    };
  }
}
