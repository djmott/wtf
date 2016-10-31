#pragma once

namespace wtf{
  namespace policy{
    template <typename _SuperT, typename _ImplT>
    class wm_move : public _SuperT{

      
    public:

      enum class wm_size_flags{
        hide = SIZE_MAXHIDE,
        maximized = SIZE_MAXIMIZED,
        show = SIZE_MAXSHOW,
        minimized = SIZE_MINIMIZED,
        restored = SIZE_RESTORED,
      };

    protected:

      virtual void on_wm_move(const point<coord_frame::client>&, bool&) {}

      explicit wm_move(iwindow * pParent) : _SuperT(pParent){}

      virtual void handle_msg(window_message& msg) override{
        if (WM_MOVE == msg.umsg) on_wm_move(point<coord_frame::client>(LOWORD(msg.lparam), HIWORD(msg.lparam)));
        _SuperT::handle_msg(msg);
      }

    };

  }
}