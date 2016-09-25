#pragma once

namespace wtf{
  struct message : MSG{
    bool get(HWND hwnd=0, UINT msgmin=0, UINT msgmax=0){
      return (exception::throw_lasterr_if(::GetMessage(this, hwnd, msgmin, msgmax), [](BOOL b){return -1 == b; }) ? true : false);
    }
    void translate() const { ::TranslateMessage(this); }
    void dispatch() const { ::DispatchMessage(this); }
    bool peek(HWND hwnd = 0, UINT msgmin = 0, UINT msgmax = 0, UINT remove = PM_NOREMOVE){
      return (exception::throw_lasterr_if(::PeekMessage(this, hwnd, msgmin, msgmax, remove), [](BOOL b){ return FALSE == b; }) ? true : false);
    }
    int pump(){
      while (get()){
        translate();
        dispatch();
      }
      return 0;
    }

  };
}