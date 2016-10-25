#pragma once


#if !defined(DCX_NODELETERGN ) 
#define DCX_NODELETERGN 0x00040000
#endif
#if !defined(DCX_USESTYLE)
#define DCX_USESTYLE 0x00010000
#endif

namespace wtf{

    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::wm_ncpaint, _Policies...> : public window<_ImplT, _Policies...>{
      using __super_t = window<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window;
    public:

    protected:

      virtual void on_wm_ncpaint(const device_context&, const rect<coord_frame::client>&){}

      explicit window(iwindow * pParent) : __super_t(pParent){}

      LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam){
        if (WM_NCPAINT == umsg){
          if (1==wparam){
            auto oDC = device_context::get_dcex(*this, DCX_WINDOW | DCX_USESTYLE | DCX_CLIPSIBLINGS | DCX_CLIPCHILDREN);
            auto oWindow = rect<coord_frame::screen>::get(*this);

            oWindow.offset(oWindow.position());
            rect<coord_frame::client> oClient(oWindow);
            on_wm_ncpaint(oDC, oClient);

          } else{

            auto oWindow = rect<coord_frame::screen>::get(*this);

            auto oRegion = region::attach((HRGN)wparam);
            oRegion.offset(oWindow.position());

            auto oDC = device_context::get_dcex(*this, oRegion, DCX_EXCLUDERGN | DCX_WINDOW | DCX_USESTYLE | DCX_CLIPSIBLINGS | DCX_CLIPCHILDREN);

            oWindow.offset(oWindow.position());
            rect<coord_frame::client> oClient(oWindow);
            on_wm_ncpaint(oDC, oClient);
          }
        }
        return __super_t::handle_message(hwnd, umsg, wparam, lparam);
      }

    };
  }
