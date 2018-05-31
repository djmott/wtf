/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once
namespace wtf{

  enum class scroll_visibilities{
    on,
    off,
    as_needed,
  };

  namespace policy{

    template <typename _SuperT>
    struct isa_scroll_bar : _SuperT{

      int min() const{ return _min; }
      void min(int newval){
        _min = newval;
        value(value());
      }

      int max() const{ return _max; }
      void max(int newval){
        _max = newval;
        value(value());
      }

      int value() const noexcept { return _value; }

      void value(int newval){
        if (newval < _min) newval = _min;
        if (newval > _max) newval = _max;
        _value = newval;
        _SuperT::invalidate();
      }

      int small_step() const{ return _small_step; }
      void small_step(int newval){
        _small_step = newval;
      }

      int big_step() const{ return _big_step; }
      void big_step(int newval){
        _big_step = newval;
      }

      callback<void(int prev_val)> OnValueChanged;

    protected:

      explicit isa_scroll_bar(window * pParent) : _SuperT(pParent){}

      virtual void on_value_changed(int prev_val){ OnValueChanged(prev_val); }

    private:


      enum button_down_on{
        none,
        step_inc,
        step_dec,
        page_inc,
        page_dec,
        slide,
      };

      void on_wm_mouse_move(const mouse_msg_param<coord_frame::client>& param) override{
        if (_down != button_down_on::slide) return _SuperT::on_wm_mouse_move(param);
        auto oClient = rect<coord_frame::client>::get(*this);
        auto prev_val = _value;
        if (orientations::vertical == _SuperT::orientation()){
          auto iNewTop = param.position.y;
          if (iNewTop < (_button_size * 3 / 2)){
            value(_min);
          } else if (iNewTop > (oClient.bottom - (_button_size * 2))){
            value(_max);
          } else{
            value(iNewTop * (_max - _min) / (oClient.bottom - (_button_size * 2)));
          }
        } else{
          auto iNewLeft = param.position.x;

          if (iNewLeft < (_button_size * 3 / 2)){
            value(_min);
          } else if (iNewLeft > (oClient.right - (_button_size * 2))){
            value(_max);
          } else{
            value(iNewLeft * (_max - _min) / (oClient.right - (_button_size * 2)));
          }
        }
        if (_value != prev_val) on_value_changed(prev_val);
        _SuperT::on_wm_mouse_move(param);
      }

      void on_wm_paint(const wtf::_::device_context& dc, const wtf::_::paint_struct& ps) override{
        auto iExtent = _max - _min;
        if (!iExtent) return _SuperT::on_wm_paint(dc, ps);
        rect<coord_frame::client> _slider;
        const rect<coord_frame::client>& client = ps.client();
        if (orientations::vertical == _SuperT::orientation()){
          _button_size = client.right - 1;
          _step_dec = { 0,0,_button_size, _button_size };
          _step_inc = { 0, client.bottom - _button_size - 1, _button_size, client.bottom - 1 };
          _arrow_dec = { _step_dec.left + 5, _step_dec.top + 5, _step_dec.right - 5, _step_dec.bottom - 5 };
          _arrow_inc = { _step_inc.left + 5, _step_inc.top + 5, _step_inc.right - 5, _step_inc.bottom - 5 };
          wtf::_::effects::draw_arrow(dc, _arrow_dec, quadrants::top, _shadow, _fill);
          wtf::_::effects::draw_arrow(dc, _arrow_inc, quadrants::bottom, _shadow, _fill);
          auto iSlideRange = (client.bottom - (_button_size * 3)) - 1;
          auto iValRange = _max - _min;
          auto iNewTop = 1 + _button_size + ((_value * iSlideRange) / iValRange);
          if ((2 + iNewTop + (_button_size * 2)) >= client.bottom) iNewTop = (client.bottom - (_button_size * 2)) - 2;

          _slider = { 0,iNewTop,_button_size,iNewTop + _button_size };
        } else{
          _button_size = client.bottom - 1;
          _step_dec = { 0,0,_button_size, _button_size };
          _step_inc = { client.right - _button_size - 1, 0,  client.right - 1, _button_size };
          _arrow_dec = { _step_dec.left + 5, _step_dec.top + 5, _step_dec.right - 5, _step_dec.bottom - 5 };
          _arrow_inc = { _step_inc.left + 5, _step_inc.top + 5, _step_inc.right - 5, _step_inc.bottom - 5 };
          wtf::_::effects::draw_arrow(dc, _arrow_dec, quadrants::left, _shadow, _fill);
          wtf::_::effects::draw_arrow(dc, _arrow_inc, quadrants::right, _shadow, _fill);

          auto iSlideRange = (client.right - (_button_size * 3)) - 1;
          auto iValRange = _max - _min;
          auto iNewLeft = 1 + _button_size + ((_value * iSlideRange) / iValRange);
          if ((2 + iNewLeft + (_button_size * 2)) >= client.right) iNewLeft = (client.right - (_button_size * 2)) - 2;

          _slider = { iNewLeft,0,iNewLeft + _button_size,_button_size };
        }
        wtf::_::effects::draw_border(dc, _step_dec, (_down == step_dec ? border_styles::lowered : border_styles::raised), _highlight, _shadow);
        wtf::_::effects::draw_border(dc, _step_inc, (_down == step_inc ? border_styles::lowered : border_styles::raised), _highlight, _shadow);
        wtf::_::effects::draw_border(dc, _slider, (_down == slide ? border_styles::lowered : border_styles::raised), _highlight, _shadow);
        _SuperT::on_wm_paint(dc, ps);
      }

      void on_wm_mouse_down(const mouse_msg_param<coord_frame::client>& param) override{
        if (mouse_buttons::left != param.button) return _SuperT::on_wm_mouse_down(param);
        ::SetCapture(*this);
        _down = button_down_on::none;
        if (_step_dec.is_in(param.position)){
          _down = button_down_on::step_dec;
        } else if (_step_inc.is_in(param.position)){
          _down = button_down_on::step_inc;
        } else if (_page_dec.is_in(param.position)){
          _down = button_down_on::page_dec;
        } else if (_page_inc.is_in(param.position)){
          _down = button_down_on::page_inc;
        } else{
          _down = button_down_on::slide;
        }
        _SuperT::invalidate();
        on_wm_click(param);
        _SuperT::on_wm_mouse_down(param);
      }

      void on_wm_click(const mouse_msg_param<coord_frame::client>& param) override{
        if (mouse_buttons::left != param.button) return _SuperT::on_wm_mouse_down(param);
        auto prevval = _value;
        if (button_down_on::step_inc == _down){
          value(_value + _small_step);
        } else if (button_down_on::step_dec == _down){
          value(_value - _small_step);
        } else if (button_down_on::page_inc == _down){
          value(_value + _big_step);
        } else if (button_down_on::page_dec == _down){
          value(_value - _big_step);
        }
        if (prevval != _value) on_value_changed(prevval);
        _SuperT::on_wm_click(param);
      }

      void on_wm_mouse_up(const mouse_msg_param<coord_frame::client>& param) override{
        ::ReleaseCapture();
        _down = button_down_on::none;
        _SuperT::invalidate();
        _SuperT::on_wm_mouse_up(param);
      }

      button_down_on _down = none;
      int _button_size = 20;
      int _small_step = 1;
      int _big_step = 1;
      int _value = 0;
      int _max = 100;
      int _min = 0;
      pen _highlight = pen::create(pen::style::solid, 1, system_colors::button_highlight);
      pen _shadow = pen::create(pen::style::solid, 1, system_colors::button_shadow);
      brush _fill = brush::solid_brush(system_colors::button_shadow);
      rect<coord_frame::client> _step_inc{ 0,0,0,0 };
      rect<coord_frame::client> _step_dec{ 0,0,0,0 };
      rect<coord_frame::client> _arrow_inc{ 0,0,0,0 };
      rect<coord_frame::client> _arrow_dec{ 0,0,0,0 };
      rect<coord_frame::client> _page_inc{ 0,0,0,0 };
      rect<coord_frame::client> _page_dec{ 0,0,0,0 };
    };

  }

  namespace _{
    template <> struct policy_traits<policy::isa_scroll_bar>{
      using requires = policy_list<policy::isa_panel, policy::wm_mouse_wheel, policy::has_repeat_click, policy::has_orientation, policy::wm_mouse_move>;
    };

  }


  struct scroll_bar : window_impl<scroll_bar, policy::isa_scroll_bar>{
    explicit scroll_bar(window * pParent) noexcept : window_impl(pParent){}
  };

}