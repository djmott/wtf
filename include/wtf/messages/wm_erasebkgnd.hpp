#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT>
    class wm_erasebkgnd : public _SuperT{
      
    protected:

      virtual void on_wm_erasebkgnd(const device_context&, const rect<coord_frame::client>&) = 0{}

      explicit wm_erasebkgnd(iwindow * pParent) : _SuperT(pParent){}

      void handle_msg(window_message& msg) override{
        if (WM_ERASEBKGND == msg.umsg){
          on_wm_erasebkgnd(*reinterpret_cast<const device_context *>(msg.lparam), rect<coord_frame::client>::get(*this));
          msg.lresult = TRUE;
          msg.bhandled = true;
        }
        _SuperT::handle_msg(msg);
      }

    };
  }
}