#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT, typename _ImplT>
    class wm_showwindow : public _SuperT{

    public:

      enum class visibility_change_flag{
        show_window = 0,
        other_unzoom = SW_OTHERUNZOOM,
        other_zoom = SW_OTHERZOOM,
        parent_closing = SW_PARENTCLOSING,
        parent_opening = SW_PARENTOPENING,
      };

    protected:

      explicit wm_showwindow(iwindow * pParent) : _SuperT(pParent){}

      virtual void on_wm_showwindow(visibility_change_flag) {}

      virtual void handle_msg(window_message& msg) override{
        if (WM_SHOWWINDOW == msg.umsg) on_wm_showwindow(static_cast<visibility_change_flag>(msg.lparam));
        _SuperT::handle_msg(msg);
      }
    };

  }
}