#pragma once

namespace wtf{
  namespace policy{

    template<typename _SuperT, typename _ImplT>
    struct has_create : _SuperT{


    protected:
      virtual void wm_create() {}

      has_create(window<void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND, UINT umsg, WPARAM, LPARAM, bool &){
        if (WM_CREATE == umsg){
          wm_create();
        }
        return 0;
      }
    };
  }
}
