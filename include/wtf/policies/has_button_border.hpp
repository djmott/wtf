
#pragma once

namespace wtf{
  namespace policy{

    template <typename _SuperT, typename _ImplT> 
    struct has_button_border : _SuperT::window_type::template add_policy<policy::has_border, messages::wm_create,
      messages::wm_mouse_down, messages::wm_mouse_up, messages::wm_mouse_leave, messages::wm_mouse_move>
    {
      using mouse_msg_param = messages::mouse_msg_param;

      has_button_border() : _SuperT(){
      }
    protected:

      using _super_t = typename _SuperT::window_type::template add_policy<policy::has_border, messages::wm_create,
        messages::wm_mouse_down, messages::wm_mouse_up, messages::wm_mouse_leave, messages::wm_mouse_move>;

      has_button_border(window<void> * pParent) : _super_t(pParent){}

      virtual LRESULT on_wm_create(bool& bhandled) override{ 
        border_style(border_styles::raised); 
        return _super_t::on_wm_create(bhandled);
      };

      virtual LRESULT on_wm_mouse_move(const mouse_msg_param& m, bool& bhandled) override{
        if (!_Down || mouse_msg_param::buttons::left != m.button) return _super_t::on_wm_mouse_move(m, bhandled);
        if (!rect<coord_frame::client>::get(*this).is_in(m.position)){
          ::ReleaseCapture();
          _Down = false;
          border_style(border_styles::raised);
          refresh();
        }
        return _super_t::on_wm_mouse_move(m, bhandled);
      };

      virtual LRESULT on_wm_mouse_down(const mouse_msg_param& m, bool& bhandled) override{
        if (mouse_msg_param::buttons::left != m.button) return _super_t::on_wm_mouse_down(m, bhandled);
        _Down = true;
        border_style(border_styles::lowered);
        ::SetCapture(*this);
        refresh();
        return _super_t::on_wm_mouse_down(m, bhandled);
      };

      virtual LRESULT on_wm_mouse_up(const mouse_msg_param& m, bool& bhandled) override{
        if (!_Down || mouse_msg_param::buttons::left != m.button) return _super_t::on_wm_mouse_up(m, bhandled);
        _Down = false;
        border_style(border_styles::raised);
        refresh();
        ::ReleaseCapture();
        return _super_t::on_wm_mouse_up(m, bhandled);
      };

      virtual LRESULT on_wm_mouse_leave(bool& bhandled){ return _super_t::on_wm_mouse_leave(bhandled); }

    private:
      bool _Down = false;
    };
  }
}