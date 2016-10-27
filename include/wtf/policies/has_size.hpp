#pragma once

namespace wtf {
    /** has_size
    * Adds members to reposition/resize and events to capture them
    */
    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::has_size, _Policies...> 
      : public window_impl<_ImplT, _Policies...>
    {
      using __super_t = window_impl<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window_impl;
    public:

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

      explicit window(iwindow * pParent) : __super_t(pParent){}


    };

  }
