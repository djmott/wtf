#pragma once

namespace wtf {
    /** has_show
    * Adds members to change the display state and capture display state changes
    */
    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::has_show, _Policies...> 
      : public window_impl<_ImplT, _Policies...>
    {
      using __super_t = window_impl<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window_impl;
    public:


      virtual void show() { ::ShowWindow(*this, SW_SHOW); }

      virtual void hide() { ::ShowWindow(*this, SW_HIDE); }

    protected:

      explicit window(iwindow * pParent) : __super_t(pParent){}

    };

  }
