#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT, typename _ImplT>
    class wm_setcursor : public _SuperT{

      
    public:


    protected:

      virtual void on_wm_setcursor(wm_nchittest_flags) {}

      explicit wm_setcursor(iwindow * pParent) : _SuperT(pParent){}

      virtual void handle_msg(window_message& msg) override{
        if (WM_SETCURSOR == msg.umsg){
          on_wm_setcursor(static_cast<wm_nchittest_flags>(LOWORD(msg.lparam)));
          msg.lresult = TRUE;
          msg.bhandled = true;
        }
        _SuperT::handle_msg(msg);
      }
    };
  }
}