#pragma once

namespace wtf {

    /** has_titlebar
    * Adds titlebar text features to windows
    */
    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::has_titlebar, _Policies...> : public window<_ImplT, _Policies...>{
      using __super_t = window<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window;
    public:

      const tstring &titlebar() const {
        return _titlebar;
      }

      void titlebar(LPCTSTR newval) {
        wtf::exception::throw_lasterr_if(::SetWindowText(*this, newval), [](BOOL b) { return !b; });
        _titlebar = newval;
      }

    protected:
      explicit window(iwindow * pParent) : __super_t(pParent){}
    private:
      tstring _titlebar = _T("");
    };

  }
