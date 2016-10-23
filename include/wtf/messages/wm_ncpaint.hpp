#pragma once


#if !defined(DCX_NODELETERGN ) 
#define DCX_NODELETERGN 0x00040000
#endif
#if !defined(DCX_USESTYLE)
#define DCX_USESTYLE 0x00010000
#endif

namespace wtf{
  namespace messages{

    template <typename _SuperT, typename>
    struct wm_ncpaint : _SuperT{

    protected:

      virtual LRESULT on_wm_ncpaint(const device_context&, const rect<coord_frame::client>&, bool&) = 0{ return 0; }

      wm_ncpaint(window<void,void> * pParent) : _SuperT(pParent){}

      LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM, bool & bHandled){
        if (WM_NCPAINT == umsg){
          if (1==wparam){
            auto oDC = device_context::get_dcex(*this, DCX_WINDOW | DCX_USESTYLE | DCX_CLIPSIBLINGS | DCX_CLIPCHILDREN);
            auto oWindow = rect<coord_frame::screen>::get(*this);


            oWindow.offset(oWindow.position());
            rect<coord_frame::client> oClient(oWindow);
            return on_wm_ncpaint(oDC, oClient, bHandled);

          } else{

            auto oWindow = rect<coord_frame::screen>::get(*this);

            auto oRegion = region::attach((HRGN)wparam);
            oRegion.offset(oWindow.position());

            auto oDC = device_context::get_dcex(*this, oRegion, DCX_EXCLUDERGN | DCX_WINDOW | DCX_USESTYLE | DCX_CLIPSIBLINGS | DCX_CLIPCHILDREN);

            oWindow.offset(oWindow.position());
            rect<coord_frame::client> oClient(oWindow);
            return on_wm_ncpaint(oDC, oClient, bHandled);
          }
        }
        return 0;
      }

    };
  }
}