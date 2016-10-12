#pragma once
namespace wtf{


  struct scroll_bar : window<scroll_bar,
    policy::has_size, policy::has_show, policy::has_orientation, policy::has_paint, policy::has_timer, policy::has_border,
    policy::has_mouse_down, policy::has_mouse_leave, policy::has_mouse_move, policy::has_mouse_up, policy::has_mouse_wheel>{

    explicit scroll_bar(HWND parent) : window(parent){}


    int min() const{ return _min; }
    void min(int newval){ _min = newval; }

    int max() const{ return _max; }
    void max(int newval){ _max = newval; }

    int value() const{ return _value; }
    void value(int newval){
      if (newval < _min) newval = _min;
      if (newval > _max) newval = _max;
      _value = newval;
    }

    int step_size() const{ return _step_size; }
    void step_size(int newval){ _step_size = newval; }

    int page_size() const{ return _page_size; }
    void page_size(int newval){ _page_size = newval; }

  protected:
    virtual void StepIncEvent(){}
    virtual void StepDecEvent(){}
    virtual void PageIncEvent(){}
    virtual void PageDecEvent(){}


    virtual void PaintEvent(const device_context& dc, const paint_struct&ps) override{
      rect::client_coord oClient = ps.client();
      oClient.left += border_width();
      oClient.top += border_width();
      oClient.right -= border_width();
      oClient.bottom -= border_width();
      point::client_coords::vector arrow(3);
      draw_border(dc, _step_dec);
      draw_border(dc, _step_inc);


      if (orientations::horizontal == orientation()){
        // <
        arrow[0].x = _step_dec.right; arrow[0].y = _step_dec.top;
        arrow[1].x = _step_dec.left; arrow[1].y = _step_dec.top + ((_step_dec.bottom - _step_dec.top) / 2);
        arrow[2].x = _step_dec.right; arrow[2].y = _step_dec.bottom;
        dc.fill(arrow, _black_pen, _black_brush);
        // >
        arrow[0].x = _step_inc.left; arrow[0].y = _step_inc.top;
        arrow[1].x = _step_inc.right; arrow[1].y = _step_inc.top + ((_step_inc.bottom - _step_inc.top) / 2);
        arrow[2].x = _step_inc.left; arrow[2].y = _step_inc.bottom;
        dc.fill(arrow, _black_pen, _black_brush);
      } else{
/*
        // v
        arrow[0].x = 5; arrow[0].y = 5;
        arrow[1].x = client.right - 5; arrow[1].y = 5;
        arrow[2].x = client.right / 2; arrow[2].y = client.bottom - 5;
        dc.fill(arrow, _outline, _fill);
        // ^
        arrow[0].x = client.right / 2; arrow[0].y = 5;
        arrow[1].x = client.right - 5; arrow[1].y = client.bottom - 5;
        arrow[2].x = 5; arrow[2].y = client.bottom - 5;
        dc.fill(arrow, _outline, _fill);
*/
      }


    }

    virtual void ResizedEvent(wm_size_flags, const point::client_coords& p) override{
      auto iExtent = _max - _min;

      rect::client_coord oClient(border_width(), border_width(), p.x - border_width() * 2, p.y - border_width() * 2);

      if (orientations::horizontal == orientation()){
        auto iButtonWidth = oClient.bottom - oClient.top;
        _step_dec = rect::client_coord(oClient.left, oClient.top, oClient.left + iButtonWidth, oClient.bottom);
        _step_inc = rect::client_coord(oClient.right - iButtonWidth, oClient.top, oClient.right, oClient.bottom);
        auto iPosRange = (oClient.right - oClient.left) - (iButtonWidth * 3);
        auto iSliderPos = iButtonWidth + ((_value * iPosRange) / iExtent);
        _slider = rect::client_coord(iSliderPos, oClient.top, iButtonWidth, iButtonWidth);
        _page_dec = rect::client_coord(_step_dec.right, oClient.top, iSliderPos, iButtonWidth);
        _page_inc = rect::client_coord(iSliderPos + iButtonWidth, oClient.top, oClient.right - iSliderPos - iButtonWidth, iButtonWidth);
      } else{

      }
      _step_dec.left += 4; _step_dec.top += 4; _step_dec.right -= 4; _step_dec.bottom -= 4;
      _step_inc.left += 4; _step_inc.top += 4; _step_inc.right -= 4; _step_inc.bottom -= 4;
    }

    rect::client_coord _step_inc;
    rect::client_coord _step_dec;

    rect::client_coord _page_inc;
    rect::client_coord _page_dec;

    rect::client_coord _slider;

    int _min = 0;
    int _max = 100;
    int _value = 0;
    int _step_size = 1;
    int _page_size = 10;

    pen _black_pen = pen::create(pen::style::solid, 1, rgb(0,0,0));
    brush _black_brush = brush::solid_brush(rgb(0, 0, 0));

  };


#if 0
  //overweight scrollbar can be much thinner
  struct overweight_scroll_bar : window<overweight_scroll_bar, policy::has_size, policy::has_show, policy::has_orientation>{

    explicit overweight_scroll_bar(HWND pParent)
      : window(pParent, true),
      _inc(*this, true),
      _dec(*this, false),
      _page_inc(*this, true),
      _page_dec(*this, false),
      _slider(*this){}


    orientations orientation() const{ return _orientation; }
    void orientation(orientations newval){
      _orientation = newval;
      ResizedEvent(wm_size_flags::restored, rect::client_coord::get(*this).dimensions());
    }

    callback<void()> OnChanged;

    int min() const{ return _min; }
    void min(int newval){ _min = newval; }

    int max() const{ return _max; }
    void max(int newval){ _max = newval; }

    int value() const{ return _value; }
    void value(int newval){
      if (newval < _min) newval = _min;
      if (newval > _max) newval = _max;
      _value = newval;
      ResizedEvent(wm_size_flags::restored, point::client_coords::get_size(*this));
    }

    int small_step() const{ return _small_step; }
    void small_step(int newval){ _small_step = newval; }

    int big_step() const{ return _big_step; }
    void big_step(int newval){ _big_step = newval; }

  protected:

    friend struct value_step_button;

    virtual void IncrementEvent(){
      auto iOriginal = _value;
      value(_value + _small_step);
      if (_value == iOriginal) return;
      poke();
      OnChanged();
    }

    virtual void DecrementEvent(){
      auto iOriginal = _value;
      value(_value - _small_step);
      if (_value == iOriginal) return;
      OnChanged();
    }

    virtual void PageUpEvent(){
      auto iOriginal = _value;
      value(_value + _big_step);
      if (_value == iOriginal) return;
      OnChanged();
    }

    virtual void PageDownEvent(){
      auto iOriginal = _value;
      value(_value - _big_step);
      if (_value == iOriginal) return;
      OnChanged();
    }

    struct value_step_button : window<value_step_button, policy::has_button_border, policy::has_repeat_click, policy::has_size, policy::has_paint, policy::has_click>{
      bool _is_increment;
      overweight_scroll_bar& _parent;

      value_step_button(overweight_scroll_bar& Parent, bool IsIncrementer) :
        window(Parent), _parent(Parent), _is_increment(IsIncrementer){}

      virtual void ClickEvent(const policy::mouse_event& m) override{
        if (policy::mouse_event::buttons::left != m.button) return;
        if (_is_increment) _parent.IncrementEvent();
        else _parent.DecrementEvent();
      }

      virtual void PaintEvent(const device_context& dc, const paint_struct&ps) override{
        window::PaintEvent(dc, ps);
        auto client = ps.client();
        point::client_coords::vector arrow(3);
        if (orientations::horizontal == _parent._orientation){
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
        dc.fill(arrow, _parent._outline, _parent._fill);
      }
    };

    struct value_page_button : window<value_step_button, policy::has_repeat_click, policy::has_size, policy::has_paint, policy::has_click>{
      value_page_button(overweight_scroll_bar& parent, bool IsIncrement) : window(parent), _parent(parent), _is_increment(IsIncrement){}

      virtual void ClickEvent(const policy::mouse_event& m) override{
        if (policy::mouse_event::buttons::left != m.button) return;
        if (_is_increment) _parent.PageUpEvent();
        else _parent.PageDownEvent();
      }

      overweight_scroll_bar& _parent;
      bool _is_increment;
    };

    struct slider : button{
      slider(overweight_scroll_bar& parent) : button(parent){}
    }_slider;


    virtual void ResizedEvent(wm_size_flags, const point::client_coords& p) override{
      auto iExtent = _max - _min;

      if (orientations::horizontal == _orientation){
        auto iStepWidth = p.y;
        _dec.move(0, 0, iStepWidth, iStepWidth);
        _inc.move(p.x - iStepWidth, 0, iStepWidth, iStepWidth);
        auto iPosRange = p.x - iStepWidth - (iStepWidth * 2);
        auto iSliderPos = iStepWidth + ((_value * iPosRange) / iExtent);
        _slider.move(iSliderPos, 0, iStepWidth, iStepWidth);
        _page_dec.move(iStepWidth, 0, iSliderPos, iStepWidth);
        _page_inc.move(iSliderPos + iStepWidth, 0, p.x - iSliderPos - (iStepWidth * 2), iStepWidth);
      } else{

      }
    }

    value_step_button _inc;
    value_step_button _dec;
    value_page_button _page_inc;
    value_page_button _page_dec;
    brush _background_brush = brush::system_brush(system_colors::inactive_border);
    pen _outline = pen::create(pen::style::solid, 1, system_colors::button_text);
    brush _fill = brush::system_brush(system_colors::button_text);
    int _min = 0;
    int _max = 100;
    int _value = 50;
    int _small_step = 1;
    int _big_step = 10;
  };
#endif
}
