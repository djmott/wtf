#pragma once


#if !defined(DCX_NODELETERGN ) 
#define DCX_NODELETERGN 0x00040000
#endif
#if !defined(DCX_USESTYLE)
#define DCX_USESTYLE 0x00010000
#endif

namespace wtf{
  namespace policy{
    template <typename _SuperT, typename _ImplT>
    class wm_ncpaint : public _SuperT{

      

    protected:

      virtual void on_wm_ncpaint(const device_context&, const rect<coord_frame::client>&) {}

      explicit wm_ncpaint(iwindow * pParent) : _SuperT(pParent){}

      void handle_msg(window_message& msg) override{
        if (WM_NCPAINT == msg.umsg){
          if (1 == msg.wparam){
            auto oDC = device_context::get_dcex(*this, DCX_WINDOW | DCX_USESTYLE | DCX_CLIPSIBLINGS | DCX_CLIPCHILDREN);
            auto oWindow = rect<coord_frame::screen>::get(*this);

            oWindow.offset(oWindow.position());
            rect<coord_frame::client> oClient(oWindow);
            on_wm_ncpaint(oDC, oClient);

          } else{

            auto oWindow = rect<coord_frame::screen>::get(*this);

            auto oRegion = region::attach((HRGN)msg.wparam);
            oRegion.offset(oWindow.position());

            auto oDC = device_context::get_dcex(*this, oRegion, DCX_EXCLUDERGN | DCX_WINDOW | DCX_USESTYLE | DCX_CLIPSIBLINGS | DCX_CLIPCHILDREN);

            oWindow.offset(oWindow.position());
            rect<coord_frame::client> oClient(oWindow);
            on_wm_ncpaint(oDC, oClient);
          }
        }
        _SuperT::handle_msg(msg);
      }

    };
  }
}