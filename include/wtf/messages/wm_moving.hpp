#pragma once

namespace wtf{

  namespace policy{
    template <typename _ImplT, typename _SuperT>
    class wm_moving : public _SuperT{

      
    public:

    protected:
      virtual void on_wm_moving(rect<coord_frame::screen>&) = 0{}

      explicit window(iwindow * pParent) : _SuperT(pParent){}

      virtual void handle_msg(window_message& msg) override{
        if (WM_MOVING == msg.umsg) on_wm_moving(*reinterpret_cast<rect<coord_frame::screen>*>(msg.lparam));
      }

    };

  }
}