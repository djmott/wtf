#pragma once
namespace wtf{

  template <typename _ImplT, template <typename> typename ... _PolicyListT> 
  struct form : window<_ImplT, wm_paint, has_size, has_close, has_show, has_titlebar, _PolicyListT...>{
    static const DWORD ExStyle = WS_EX_OVERLAPPEDWINDOW;
    static const DWORD Style = WS_OVERLAPPEDWINDOW;
  };

}