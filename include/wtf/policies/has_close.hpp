#pragma once

namespace wtf {
    /** has_close
    * Add members to close a UI element and events to capture it
    */
    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::has_close, _Policies...> 
      : public window_impl<_ImplT, _Policies...>
    {
      using __super_t = window_impl<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window_impl;
    public:

      void close() { CloseWindow(*this); }


    protected:

      explicit window(iwindow * pParent) : __super_t(pParent){}

    };
  }
