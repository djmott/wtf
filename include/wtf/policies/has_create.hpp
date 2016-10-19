#pragma once

namespace wtf{
  namespace policy{

    template<typename _SuperT, typename _ImplT>
    struct has_create : _SuperT{


    protected:
      virtual void OnCreate() {}

      has_create(window<void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND, UINT umsg, WPARAM, LPARAM, bool &){
        if (WM_CREATE == umsg){
          OnCreate();
        }
        return 0;
      }
    };
  }
}
