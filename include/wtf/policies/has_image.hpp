#pragma once

namespace wtf{
    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::has_image, _Policies...> 
      : public window_impl<_ImplT, _Policies...>
    {
      using __super_t = window_impl<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window_impl;
    public:

    protected:
      explicit window(iwindow * pParent) : __super_t(pParent){}
    };
  }
