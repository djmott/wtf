/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once


namespace wtf{
  namespace policy{

    template <typename _SuperT>
    struct isa_scrollbar : _SuperT{

      int min() const{ return _min; }
      void min(int newval){ 
        _min = newval;
        on_wm_size(_size);
      }

      int max() const{ return _max; }
      void max(int newval){
        _max = newval;
        on_wm_size(_size);
      }

      int value() const{ return _value; }
      void value(int newval){
        if (newval < _min) newval = _min;
        if (newval > _max) newval = _max;
        _value = newval;
        on_wm_size(_size);
      }

      int small_step() const{ return _small_step; }
      void small_step(int newval){ 
        _small_step = newval; 
        on_wm_size(_size);
      }

      int big_step() const{ return _big_step; }
      void big_step(int newval){ 
        _big_step = newval;
        on_wm_size(_size);
      }

    protected:
      friend struct value_step_button;
      friend struct value_page_button;

      explicit isa_scrollbar(window * pParent) : _SuperT(pParent), _inc(this, true), _dec(this, false),
        _page_inc(this, true), _page_dec(this, false), _slider(this){}

      void on_wm_create() override{
        _page_dec.zorder(_slider);
        _page_inc.zorder(_slider);
        _SuperT::on_wm_create();
      }

      void on_wm_size(const point<coord_frame::client>& p) override{
        auto iExtent = _max - _min;
        _size = p;
        if (orientations::horizontal == _SuperT::orientation()) {
          auto iStepWidth = p.y;
          _dec.move(0, 0, iStepWidth, iStepWidth);
          _inc.move(p.x - iStepWidth, 0, iStepWidth, iStepWidth);
          auto iPosRange = p.x - iStepWidth - (iStepWidth * 2);
          auto iSliderPos = iStepWidth + ((_value * iPosRange) / iExtent);
          _slider.move(iSliderPos, 0, iStepWidth, iStepWidth);
          _page_dec.move(iStepWidth, 0, iSliderPos, iStepWidth);
          _page_inc.move(iSliderPos + iStepWidth, 0, p.x - iSliderPos - (iStepWidth * 2), iStepWidth);
        } else {
          auto iStepHeight = p.x;
          _dec.move(0, 0, iStepHeight, iStepHeight);
          _inc.move(0, p.y - iStepHeight, iStepHeight, iStepHeight);
          auto iPosRange = p.y - iStepHeight - (iStepHeight * 2);
          auto iSliderPos = iStepHeight + ((_value * iPosRange) / iExtent);
          _slider.move(0, iSliderPos, iStepHeight, iStepHeight);
          _page_dec.move(0, iStepHeight, iStepHeight, iSliderPos - iStepHeight);
          _page_inc.move(0, iSliderPos + iStepHeight, iStepHeight, p.y - iSliderPos - (iStepHeight * 2));
        }
        _SuperT::on_wm_size(p);
      };

      virtual void StepIncEvent(){ value(_value + _small_step); }

      virtual void StepDecEvent(){ value(_value - _small_step); }

      virtual void PageUpEvent(){ value(_value + _big_step); }

      virtual void PageDownEvent(){ value(_value - _big_step); }

      void on_wm_mouse_wheel(int16_t delta, const mouse_msg_param& m) override{
        if (delta > 0){
          StepIncEvent();
        } else{
          StepDecEvent();
        }
        _SuperT::on_wm_mouse_wheel(delta, m);
      }


      struct value_step_button: window_impl<value_step_button, policy::isa_button, policy::has_repeat_click>{
        using __super_t = window_impl<value_step_button, policy::isa_button, policy::has_repeat_click>;
        bool _is_increment;
        isa_scrollbar * _parent;

        value_step_button(isa_scrollbar * pParent, bool IsIncrementer) : 
          __super_t(pParent), _parent(pParent), _is_increment(IsIncrementer){}

        void on_wm_click(const mouse_msg_param& m) override{
          if (mouse_msg_param::buttons::left == m.button){
            if (_is_increment) _parent->StepIncEvent();
            else _parent->StepDecEvent();
          }
          __super_t::on_wm_click(m);
        };

        void on_wm_paint(const _::device_context& dc, const _::paint_struct&ps) override{
          auto client = ps.client();
          point<coord_frame::client>::vector arrow(3);
          if (orientations::horizontal == _parent->_orientation){
            if (_is_increment){ // >
              arrow[0].x = 5; arrow[0].y = 5;
              arrow[1].x = client.right - 5; arrow[1].y = client.bottom / 2;
              arrow[2].x = 5; arrow[2].y = client.bottom - 5;
            } else{ // <
              arrow[0].x = 5; arrow[0].y = client.bottom / 2;
              arrow[1].x = client.right - 5; arrow[1].y = 5;
              arrow[2].x = arrow[1].x; arrow[2].y = client.bottom - 5;
            }
          } else{
            if (_is_increment){ // v
              arrow[0].x = 5; arrow[0].y = 5;
              arrow[1].x = client.right - 5; arrow[1].y = 5;
              arrow[2].x = client.right / 2; arrow[2].y = client.bottom - 5;
            } else{ // ^
              arrow[0].x = client.right / 2; arrow[0].y = 5;
              arrow[1].x = client.right - 5; arrow[1].y = client.bottom - 5;
              arrow[2].x = 5; arrow[2].y = client.bottom - 5;
            }
          }
          dc.fill(arrow, _parent->_outline, _parent->_fill);
          __super_t::on_wm_paint(dc, ps);
        };

      };


      struct value_page_button : window_impl<value_page_button, policy::isa_label>{

        using __super_t = window_impl<value_page_button, policy::isa_label>;

        explicit value_page_button(isa_scrollbar * pParent, bool IsIncrement) 
          : __super_t(pParent), _parent(pParent), _is_increment(IsIncrement){}

        void on_wm_click(const mouse_msg_param& m) override{
          if (mouse_msg_param::buttons::left != m.button) return __super_t::on_wm_click(m);
          if (_is_increment) _parent->PageUpEvent();
          else _parent->PageDownEvent();
          __super_t::on_wm_click(m);
        };

        isa_scrollbar * _parent;
        bool _is_increment;
      };


      struct slider : window_impl<slider, policy::isa_button>{
        using __super_t = window_impl<slider, policy::isa_button>;
        explicit slider(isa_scrollbar * pParent) : __super_t(pParent){}
      }_slider;



      value_step_button _inc;
      value_step_button _dec;
      value_page_button _page_inc;
      value_page_button _page_dec;
      brush _background_brush = brush::system_brush(system_colors::inactive_border);
      pen _outline = pen::create(pen::style::solid, 1, system_colors::button_text);
      brush _fill = brush::system_brush(system_colors::button_text);
      point<coord_frame::client> _size;
      int _min = 0;
      int _max = 100;
      int _value = 50;
      int _small_step = 1;
      int _big_step = 10;
    };
  }

  namespace _{

    template <> struct policy_traits<policy::isa_scrollbar>{
      using requires = policy_list<policy::isa_panel, policy::wm_mouse_wheel, policy::has_orientation>;
    };

  }

  struct scrollbar : window_impl<scrollbar, policy::isa_scrollbar>{
    explicit scrollbar(window * pParent) : window_impl(pParent){}
  };

}
