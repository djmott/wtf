#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT>
    class wm_moving : public _SuperT{

      
    public:

    protected:
      virtual void on_wm_moving(rect<coord_frame::screen>&) {}

      explicit window(iwindow * pParent) : _SuperT(pParent){}

      void handle_msg(window_message& msg) override{
        if (WM_MOVING == msg.umsg) on_wm_moving(*reinterpret_cast<rect<coord_frame::screen>*>(msg.lparam));
        _SuperT::handle_msg(msg);
      }

    };

  }
}