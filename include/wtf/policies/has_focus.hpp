#pragma once

namespace wtf {
  namespace policy {
    /** has_focus
    * Provides members and events related to keyboard/mouse focus
    */
    template<typename _SuperT, typename _ImplT>
    struct has_focus : _SuperT {


      void set_focus() const { wtf::exception::throw_lasterr_if(::SetFocus(*this), [](HWND h) { return !h; }); }

      bool got_focus() const { return _SuperT::_handle == ::GetFocus(); }

    protected:
      virtual void OnGotFocus(){}
      virtual void OnLostFocus(){}

      has_focus(window<void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND , UINT umsg, WPARAM , LPARAM , bool &) {
        if (WM_SETFOCUS == umsg) OnGotFocus();
        else if (WM_KILLFOCUS == umsg) OnLostFocus();
        return 0;
      }
    };
  }
}
