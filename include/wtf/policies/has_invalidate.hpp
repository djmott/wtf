#pragma once

namespace wtf{
    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::has_invalidate, _Policies...> 
      : public window_impl<_ImplT, _Policies...>
    {
      using __super_t = window_impl<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window_impl;
    public:

      void invalidate(bool EraseBackground = true){
        if (_handle){
          wtf::exception::throw_lasterr_if(
            ::InvalidateRect(_handle, nullptr, (EraseBackground ? TRUE : FALSE)), [](BOOL b){ return !b; });
        }
      }

    protected:
      explicit window(iwindow * pParent) : __super_t(pParent){}

    };
  }

