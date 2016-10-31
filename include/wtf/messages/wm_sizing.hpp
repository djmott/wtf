#pragma once

namespace wtf{
  namespace policy{
    template <typename _SuperT, typename _ImplT>
    class wm_sizing : public _SuperT{

    protected:
      virtual void on_wm_sizing(rect<coord_frame::screen>&) {}

      explicit wm_sizing(iwindow * pParent) : _SuperT(pParent){}

      virtual void handle_msg(window_message& msg) override{
        if (WM_SIZING == msg.umsg) on_wm_sizing(*reinterpret_cast<rect<coord_frame::screen>*>(msg.lparam));
        _SuperT::handle_msg(msg);
      }

    };

  }
}
