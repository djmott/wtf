#pragma once

namespace wtf{

  /** has_titlebar
  * Adds titlebar text features to windows
  */
  namespace policy{
    template <typename _SuperT>
    class has_titlebar : public _SuperT{
      
    public:

      const tstring &titlebar() const{
        return _titlebar;
      }

      void titlebar(LPCTSTR newval){
        wtf::exception::throw_lasterr_if(::SetWindowText(*this, newval), [](BOOL b){ return !b; });
        _titlebar = newval;
      }

    protected:
      explicit has_titlebar(iwindow * pParent) : _SuperT(pParent){}
    private:
      tstring _titlebar = _T("");
    };
  }
}
