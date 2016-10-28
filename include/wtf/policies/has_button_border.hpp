
#pragma once

namespace wtf{
  namespace policy{
    template <typename _ImplT, typename _SuperT>
    class has_button_border : public _SuperT{

      
    public:

    protected:


      explicit has_button_border(iwindow * pParent) : _SuperT(pParent){}

      virtual LRESULT on_wm_create(bool& bhandled) override{
        border_style(border_styles::raised);
        return _SuperT::on_wm_create(bhandled);
      };

      virtual LRESULT on_wm_mouse_move(const mouse_msg_param& m, bool& bhandled) override{
        if (!_Down || mouse_msg_param::buttons::left != m.button) return _SuperT::on_wm_mouse_move(m, bhandled);
        if (!rect<coord_frame::client>::get(*this).is_in(m.position)){
          ::ReleaseCapture();
          _Down = false;
          border_style(border_styles::raised);
          invalidate();
        }
        return _SuperT::on_wm_mouse_move(m, bhandled);
      };

      virtual LRESULT on_wm_mouse_down(const mouse_msg_param& m, bool& bhandled) override{
        if (mouse_msg_param::buttons::left != m.button) return _SuperT::on_wm_mouse_down(m, bhandled);
        _Down = true;
        border_style(border_styles::lowered);
        ::SetCapture(*this);
        invalidate();
        return _SuperT::on_wm_mouse_down(m, bhandled);
      };

      virtual LRESULT on_wm_mouse_up(const mouse_msg_param& m, bool& bhandled) override{
        if (!_Down || mouse_msg_param::buttons::left != m.button) return _SuperT::on_wm_mouse_up(m, bhandled);
        _Down = false;
        border_style(border_styles::raised);
        invalidate();
        ::ReleaseCapture();
        return _SuperT::on_wm_mouse_up(m, bhandled);
      };

      virtual LRESULT on_wm_mouse_leave(bool& bhandled) override{
        return _SuperT::on_wm_mouse_leave(bhandled);
      }

    private:
      bool _Down = false;
    };
  }
}
