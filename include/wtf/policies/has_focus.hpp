#pragma once

namespace wtf {
    /** has_focus
    * Provides members and events related to keyboard/mouse focus
    */
    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::has_focus, _Policies...> : public window<_ImplT, _Policies...>{
      using __super_t = window<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window;
    public:


      void set_focus() const { wtf::exception::throw_lasterr_if(::SetFocus(*this), [](HWND h) { return !h; }); }

      bool got_focus() const { return __super_t::_handle == ::GetFocus(); }

    protected:

      explicit window(iwindow * pParent) : __super_t(pParent){}

    };
  }
