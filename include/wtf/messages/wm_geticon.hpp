#pragma once

namespace wtf{
  enum class icon_type{
    big_icon = ICON_BIG,
    small_icon = ICON_SMALL,
    small2_icon = ICON_SMALL2,
  };

  namespace policy{
    template <typename _ImplT, typename _SuperT>
    class wm_geticon : public _SuperT{


      
    public:


    protected:

      virtual HICON on_wm_geticon(icon_type) = 0;

      explicit wm_geticon(iwindow * pParent) : _SuperT(pParent){}

      virtual void handle_msg(window_message& msg) override{
        if (WM_GETICON == msg.umsg){
          msg.lresult = reinterpret_cast<LRESULT>(on_wm_geticon(static_cast<icon_type>(msg.wparam)));
          msg.bhandled = true;
        }
      }
    };
  }
}