#pragma once
namespace wtf{

  //overweight scrollbar can be much thinner
  struct scroll_bar : window<scroll_bar, policy::has_size, policy::has_show, policy::has_orientation, 
    messages::wm_size, messages::wm_mouse_wheel, policy::has_move, messages::wm_create, policy::has_background,
    messages::wm_erasebkgnd>
  {

    using mouse_msg_param = messages::mouse_msg_param;

    explicit scroll_bar(window<void,void> * pParent)
      : window(pParent),
      _inc(this, true),
      _dec(this, false),
      _page_inc(this, true),
      _page_dec(this, false),
      _slider(this)
    {
    }

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

    int small_step() const{ return _small_step; }
    void small_step(int newval){ _small_step = newval; }

    int big_step() const{ return _big_step; }
    void big_step(int newval){ _big_step = newval; }

  protected:

    friend struct value_step_button;

    virtual LRESULT on_wm_create(bool& bHandled) override{
      return window::on_wm_create(bHandled);
    }

    virtual LRESULT on_wm_size(const point<coord_frame::client>& p, bool&bHandled) override{
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
      return window::on_wm_size(p, bHandled);
    };

    virtual void StepIncEvent(){
      auto iOriginal = _value;
      value(_value + _small_step);
      if (_value == iOriginal) return;
    }

    virtual void StepDecEvent(){
      auto iOriginal = _value;
      value(_value - _small_step);
      if (_value == iOriginal) return;
    }

    virtual void PageUpEvent(){
      auto iOriginal = _value;
      value(_value + _big_step);
      if (_value == iOriginal) return;
    }

    virtual void PageDownEvent(){
      auto iOriginal = _value;
      value(_value - _big_step);
      if (_value == iOriginal) return;
    }

    virtual LRESULT on_wm_mouse_wheel(int16_t delta, const mouse_msg_param& m, bool & bHandled) override{
      if (delta > 0){
        StepIncEvent();
      } else{
        StepDecEvent();
      }
      return window::on_wm_mouse_wheel(delta, m, bHandled);
    }


    struct value_step_button
      : window<value_step_button, policy::has_button_border, policy::has_repeat_click, messages::wm_create,
      policy::has_size, messages::wm_paint, policy::has_click, policy::has_border, policy::has_timer,
      policy::has_move, messages::wm_ncpaint, messages::wm_nccalcsize, messages::wm_mouse_down, 
      messages::wm_mouse_up, messages::wm_mouse_move, messages::wm_mouse_leave >
    {
      bool _is_increment;
      scroll_bar * _parent;

      value_step_button(scroll_bar * pParent, bool IsIncrementer) :
        window(pParent), _parent(pParent), _is_increment(IsIncrementer){

      }
      virtual void on_wm_click(const mouse_msg_param& m) override{
        if (mouse_msg_param::buttons::left != m.button) return window::on_wm_click(m);
        if (_is_increment) _parent->StepIncEvent();
        else _parent->StepDecEvent();
        window::on_wm_click(m);
      };

      virtual LRESULT on_wm_paint(const device_context& dc, const paint_struct&ps, bool& bHandled) override{
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
        return window::on_wm_paint(dc, ps, bHandled);
      };

    };

    struct value_page_button : label{

      explicit value_page_button(scroll_bar * pParent, bool IsIncrement) : label(pParent), _parent(pParent), _is_increment(IsIncrement)
      {

      }
      virtual void on_wm_click(const mouse_msg_param& m) override{
        if (mouse_msg_param::buttons::left != m.button) return;
        if (_is_increment) _parent->PageUpEvent();
        else _parent->PageDownEvent();
        label::on_wm_click(m);
      };

      scroll_bar * _parent;
      bool _is_increment;
    };

    struct slider : window<slider, policy::has_size, policy::has_move>{
      explicit slider(scroll_bar * pParent) : window(pParent){}
    }_slider;



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
}
