#pragma once

namespace wtf{
  namespace policy{

    template<typename _SuperT, typename _ImplT>
    struct has_create : _SuperT{

    protected:
      has_create(iwindow * pParent) : _SuperT(pParent){}
      virtual void CreateEvent(){}

      LRESULT handle_message(HWND, UINT umsg, WPARAM, LPARAM, bool &){
        if (WM_CREATE == umsg){
          CreateEvent();
        }
        return 0;
      }
    };
  }
}
