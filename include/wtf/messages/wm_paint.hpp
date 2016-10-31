#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT, typename _ImplT>
    class wm_paint : public _SuperT{

      
    public:

    protected:

      virtual void on_wm_paint(const device_context&, const paint_struct&){}

      explicit wm_paint(iwindow * pParent) : _SuperT(pParent){}

      virtual void handle_msg(window_message& msg) override{
        if (WM_PAINT == msg.umsg){
          auto & dc = *reinterpret_cast<const device_context *>(msg.wparam);
          auto & ps = *reinterpret_cast<const paint_struct *>(msg.lparam);
          on_wm_paint(dc, ps);
        }
        _SuperT::handle_msg(msg);
      }

    };
  }
}