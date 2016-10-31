#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT>
    class wm_size : public _SuperT{

    public:


    protected:

      explicit wm_size(iwindow * pParent) : _SuperT(pParent){}

      virtual void on_wm_size(const point<coord_frame::client>&){}

      void handle_msg(window_message& msg) override{
        if (WM_SIZE == msg.umsg) on_wm_size(point<coord_frame::client>(LOWORD(msg.lparam), HIWORD(msg.lparam)));
        _SuperT::handle_msg(msg);
      }

    };

  }
}