
#pragma once

namespace wtf{

    template <typename _ImplT, policy..._Policies> 
    class window<_ImplT, policy::has_button_border, _Policies...> 
      : public window_impl<_ImplT, _Policies...>
    {
      using __super_t = window_impl<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window_impl;
    public:

    protected:


      explicit window(iwindow * pParent) : __super_t(pParent){}

      virtual LRESULT on_wm_create(bool& bhandled) override{ 
        border_style(border_styles::raised); 
        return __super_t::on_wm_create(bhandled);
      };

      virtual LRESULT on_wm_mouse_move(const mouse_msg_param& m, bool& bhandled) override{
        if (!_Down || mouse_msg_param::buttons::left != m.button) return __super_t::on_wm_mouse_move(m, bhandled);
        if (!rect<coord_frame::client>::get(*this).is_in(m.position)){
          ::ReleaseCapture();
          _Down = false;
          border_style(border_styles::raised);
          invalidate();
        }
        return __super_t::on_wm_mouse_move(m, bhandled);
      };

      virtual LRESULT on_wm_mouse_down(const mouse_msg_param& m, bool& bhandled) override{
        if (mouse_msg_param::buttons::left != m.button) return __super_t::on_wm_mouse_down(m, bhandled);
        _Down = true;
        border_style(border_styles::lowered);
        ::SetCapture(*this);
        invalidate();
        return __super_t::on_wm_mouse_down(m, bhandled);
      };

      virtual LRESULT on_wm_mouse_up(const mouse_msg_param& m, bool& bhandled) override{
        if (!_Down || mouse_msg_param::buttons::left != m.button) return __super_t::on_wm_mouse_up(m, bhandled);
        _Down = false;
        border_style(border_styles::raised);
        invalidate();
        ::ReleaseCapture();
        return __super_t::on_wm_mouse_up(m, bhandled);
      };

      virtual LRESULT on_wm_mouse_leave(bool& bhandled) override{ 
        return __super_t::on_wm_mouse_leave(bhandled);
      }

    private:
      bool _Down = false;
    };
  }
