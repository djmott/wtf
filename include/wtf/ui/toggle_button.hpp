#pragma once

namespace wtf{

    struct toggle_button : wtf::window<toggle_button, policy::has_size, policy::has_border, 
      policy::has_click, policy::has_text, policy::has_font, policy::has_move, policy::has_background,
      messages::wm_paint, messages::wm_create, messages::wm_mouse_up, messages::wm_erasebkgnd, 
      messages::wm_mouse_down, messages::wm_ncpaint, messages::wm_nccalcsize>
    {

      using mouse_msg_param = messages::mouse_msg_param;

      toggle_button() = delete;
      toggle_button(const toggle_button&) = delete;
      toggle_button(toggle_button&&) = delete;
      toggle_button &operator=(const toggle_button &) = delete;
      toggle_button &operator=(toggle_button&&) = delete;
      virtual ~toggle_button() = default;
      explicit toggle_button(window<void,void> * pParent) : window(pParent), _button_state(button_states::up){ }

      virtual LRESULT on_wm_create(bool& bHandled) override{ 
        border_style(border_styles::raised); 
        return window::on_wm_create(bHandled);
      };

      virtual LRESULT on_wm_mouse_up(const messages::mouse_msg_param& m, bool& bHandled) override{
        if (messages::mouse_msg_param::buttons::left != m.button) return window::on_wm_mouse_up(m, bHandled);
        button_state(button_states::down == _button_state ? button_states::up : button_states::down);
        return window::on_wm_mouse_up(m, bHandled);
      };

      enum class button_states{
        up,
        down,
      };

      button_states button_state() const{ return _button_state; }
      void button_state(button_states newval) { 
        if (newval == _button_state) return;
        _button_state=newval;
        border_style((button_states::down == _button_state ? border_styles::lowered : border_styles::raised));
        invalidate();
      }

    private:


      button_states _button_state;
    };

}
