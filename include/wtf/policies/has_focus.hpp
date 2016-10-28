#pragma once

namespace wtf{
  /** has_focus
  * Provides members and events related to keyboard/mouse focus
  */
  namespace policy{
    template <typename _ImplT, typename _SuperT>
    class has_focus : public _SuperT{

      
    public:


      void set_focus() const{ wtf::exception::throw_lasterr_if(::SetFocus(*this), [](HWND h){ return !h; }); }

      bool got_focus() const{ return __super_t::_handle == ::GetFocus(); }

    protected:

      explicit has_focus(iwindow * pParent) : _SuperT(pParent){}

    };
  }
}
