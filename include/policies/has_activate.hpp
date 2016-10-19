#pragma once

namespace wtf{
  namespace policy{
    template <typename _SuperT, typename>
    struct has_activate : _SuperT{

      callback<void()> OnActivate;
      callback<void()> OnDeactivate;

    protected:
      has_activate(iwindow * pParent) : _SuperT(pParent){}


      LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM, bool &) {
        if (WM_ACTIVATE == umsg){
          if (WA_INACTIVE == wparam){
            OnDeactivate();
          } else{
            OnActivate();
          }
        }
        return 0;
      }

    };
  }
}