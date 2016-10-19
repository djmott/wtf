#pragma once

namespace wtf{
  namespace policy{

    template<typename _SuperT, typename _ImplT>
    struct has_create : _SuperT{

      callback<void()> OnCreate;

    protected:
      has_create(iwindow * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND, UINT umsg, WPARAM, LPARAM, bool &){
        if (WM_CREATE == umsg){
          OnCreate();
        }
        return 0;
      }
    };
  }
}
