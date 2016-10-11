#pragma once
namespace wtf{


    struct scroll_bar : window<scroll_bar, policy::has_border, policy::has_size, policy::has_paint, 
      policy::has_mouse, policy::has_timer, policy::has_show>
    {
      

      virtual ~scroll_bar() = default;
      scroll_bar(const scroll_bar&) = delete;
      scroll_bar &operator=(const scroll_bar &) = delete;
      scroll_bar(scroll_bar&&) = delete;
      scroll_bar &operator=(scroll_bar&&) = delete;

      explicit scroll_bar(HWND pParent)
        : window(pParent, true),
        _orientation(orientations::horizontal),
        _background_brush(brush::system_brush(system_colors::inactive_border)),
        _outline(pen::create(pen::style::solid, 1, system_colors::button_text)),
        _fill(brush::system_brush(system_colors::button_text)),
        _inc(*this, true),
        _dec(*this, false),
        _page_inc(*this, true),
        _page_dec(*this, false),
        _slider(*this),
        _min(0),
        _max(100),
        _value(50)
      {}

      enum class orientations{
        horizontal,
        vertical
      };

      orientations orientation() const{ return _orientation; }
      void orientation(orientations newval){ 
        _orientation = newval; 
        ResizedEvent(wm_size_flags::restored, rect::client_coord::get(*this).dimensions());
      }

      callback<void()> OnIncrement;
      callback<void()> OnDecrement;
      callback<void()> OnPageUp;
      callback<void()> OnPageDown;

      int min() const{ return _min; }
      void min(int newval){ _min = newval; }

      int max() const{ return _max; }
      void max(int newval){ _max = newval; }

      int value() const{ return _value; }
      void value(int newval){ _value = newval; }


    protected:

      friend struct value_step_button;

      virtual void IncrementEvent(){ OnIncrement(); }
      virtual void DecrementEvent(){ OnDecrement(); }


      struct value_step_button : window<value_step_button, policy::has_button_border, policy::has_repeat_click, policy::has_size, policy::has_paint, policy::has_click>{
        bool _IsIncrementer;
        scroll_bar& _Parent;

        value_step_button(scroll_bar& Parent, bool IsIncrementer) :
          window(Parent), _Parent(Parent), _IsIncrementer(IsIncrementer)
        {}

        virtual void ClickEvent(const point::client_coords&) override {
          if (_IsIncrementer) _Parent.IncrementEvent();
          else _Parent.DecrementEvent();
        }
        
        virtual void PaintEvent(const device_context& dc, const paint_struct&ps) override {
          window::PaintEvent(dc, ps);
          auto client = ps.client();
          point::client_coords::vector arrow(3);
          if (orientations::horizontal == _Parent._orientation){
            if (_IsIncrementer){ // >
              arrow[0].x = 5; arrow[0].y = 5;
              arrow[1].x = client.right - 5; arrow[1].y = client.bottom / 2;
              arrow[2].x = 5; arrow[2].y = client.bottom - 5;
            } else{ // <
              arrow[0].x = 5; arrow[0].y = client.bottom / 2;
              arrow[1].x = client.right - 5; arrow[1].y = 5;
              arrow[2].x = arrow[1].x; arrow[2].y = client.bottom - 5;
            }
          } else{
            if (_IsIncrementer){ // v
              arrow[0].x = 5; arrow[0].y = 5;
              arrow[1].x = client.right - 5; arrow[1].y = 5;
              arrow[2].x = client.right / 2; arrow[2].y = client.bottom - 5;
            } else{ // ^
              arrow[0].x = client.right / 2; arrow[0].y = 5;
              arrow[1].x = client.right - 5; arrow[1].y = client.bottom - 5;
              arrow[2].x = 5; arrow[2].y = client.bottom - 5;
            }
          }         
          dc.fill(arrow, _Parent._outline, _Parent._fill);
        }
      };

      struct value_page_button : label{
        value_page_button(scroll_bar& parent, bool IsIncrement) : label(parent), _parent(parent), _is_increment(IsIncrement){}

        scroll_bar& _parent;
        bool _is_increment;
      };

      struct slider : push_button{
        slider(scroll_bar& parent) : push_button(parent){}
      }_slider;



      virtual void MouseWheelEvent(event_vkeys, int16_t delta, const point::screen_coords&p) override{
        delta /= 120;
        if (delta > 0){
          for (;delta>0;--delta) _dec.ClickEvent(p.to_client(*this));
        }else {
          for (;delta<0;++delta) _inc.ClickEvent(p.to_client(*this));
        }
      }

      virtual void ResizedEvent(wm_size_flags, const point::client_coords& p) override {
        auto iValRange = _max - _min;
        
        if (orientations::horizontal == _orientation){
          _dec.move(0, 0, p.y, p.y);
          _inc.move(p.x - p.y, 0, p.y, p.y);
          auto iRepRange = p.x - p.y;//representative range
          auto iSliderPos = _value * iRepRange / iValRange;
          _slider.move(iSliderPos, 0, p.y, p.y);

        } else{
          _dec.move(0, 0, p.x, p.x);
          _inc.move(0, p.y - p.x, p.x, p.x);
        }
      }
      

      virtual void TimerEvent(UINT_PTR iTimer) override{
        if (iTimer != _ButtonDownTimer) return;
        kill_timer(_ButtonDownTimer);
        if (!_ButtonHeldDown) return;
        _ButtonDownTimer = set_timer(50);
        _ButtonHeldDown->ClickEvent(cursor::position().to_client(*this));
      }

      virtual const brush& background_brush() const override{ return _background_brush; }

      orientations _orientation;
      brush _background_brush;
      pen _outline;
      brush _fill;
      value_step_button _inc;
      value_step_button _dec;
      value_page_button _page_inc;
      value_page_button _page_dec;
      int _min = 0;
      int _max = 100;
      int _value = 50;
      UINT_PTR _ButtonDownTimer = 0;
      value_step_button * _ButtonHeldDown = nullptr;
    };

}
