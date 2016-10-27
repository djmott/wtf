#pragma once

namespace wtf{
  enum class zorders{
    bottom = 1,
    no_top_most = -2,
    top = 0,
    top_most = -1,
  };


  template <typename _ImplT, policy..._Policies>
  class window<_ImplT, policy::has_zorder, _Policies...> 
    : public window_impl<_ImplT, _Policies...>
  {
    using __super_t = window_impl<_ImplT, _Policies...>;
    template <typename, policy ... > friend class window_impl;
  public:

    void zorder(zorders pos){
      wtf::exception::throw_lasterr_if(::SetWindowPos(*this, reinterpret_cast<HWND>(pos), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOCOPYBITS), [](BOOL b){ return !b; });
    }
    void zorder(HWND insert_after){
      wtf::exception::throw_lasterr_if(::SetWindowPos(*this, insert_after, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOCOPYBITS), [](BOOL b){ return !b; });
    }
  protected:
    explicit window(iwindow * pParent) : __super_t(pParent){}

  };
}
