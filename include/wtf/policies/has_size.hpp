#pragma once

namespace wtf {
  namespace policy {
    /** has_size
    * Adds members to reposition/resize and events to capture them
    */
    template<typename _SuperT, typename _ImplT>
    struct has_size : _SuperT {

      virtual int width() const{
        auto r = rect<coord_frame::client>::get(*this);
        return r.right - r.left;
      }

      virtual int height() const{
        auto r = rect<coord_frame::client>::get(*this);
        return r.bottom - r.top;
      }


      virtual void width(int newval){
        wtf::exception::throw_lasterr_if(::SetWindowPos(*this, 0, 0, 0, newval, height(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOREPOSITION),
                                         [](BOOL b){ return !b; });
      }

      virtual void height(int newval){
        wtf::exception::throw_lasterr_if(::SetWindowPos(*this, 0, 0, 0, width(), newval, SWP_NOMOVE | SWP_NOZORDER | SWP_NOREPOSITION),
                                         [](BOOL b){ return !b; });
      }


    protected:

      virtual void wm_size(const point<coord_frame::client>&) {}

      has_size(window<void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND , UINT umsg, WPARAM wparam, LPARAM lparam, bool & bHandled) {
        if (WM_SIZE == umsg){
          wm_size( point<coord_frame::client>(LOWORD(lparam), HIWORD(lparam)));
        } 
        return 0;
      }
    private:

    };

  }
}
