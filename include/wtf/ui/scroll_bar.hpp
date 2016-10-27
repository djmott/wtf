#pragma once


namespace wtf{

  template <typename _ImplT, policy..._Policies>
  class window<_ImplT, policy::isa_scrollbar, _Policies...> 
    : public window_impl<_ImplT, _Policies..., policy::isa_panel,  policy::has_orientation,
    policy::wm_mouse_wheel>
  {
    using scrollbar_t = window<_ImplT, policy::isa_scrollbar, _Policies...>;
    using __super_t = window_impl<_ImplT, _Policies..., policy::isa_panel, policy::has_orientation,
      policy::wm_mouse_wheel>;
    using mouse_msg_param = mouse_msg_param;
  public:
    explicit window(iwindow * pParent)
      : __super_t(pParent),
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
    virtual void handle_msg(window_message& msg) override{}

    virtual void on_wm_size(const point<coord_frame::client>& p) override{
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
      __super_t::on_wm_size(p);
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

    virtual void on_wm_mouse_wheel(int16_t delta, const mouse_msg_param& m) override{
      if (delta > 0){
        StepIncEvent();
      } else{
        StepDecEvent();
      }
      __super_t::on_wm_mouse_wheel(delta, m);
    }


    struct value_step_button
      : window<value_step_button, policy::isa_button >
    {
      using __super_t = window<value_step_button, policy::isa_button >;
      bool _is_increment;
      scrollbar_t * _parent;

      value_step_button(scrollbar_t * pParent, bool IsIncrementer) :
        __super_t(pParent), _parent(pParent), _is_increment(IsIncrementer){

      }
      virtual void on_wm_click(const mouse_msg_param& m) override{
        if (mouse_msg_param::buttons::left == m.button){
          if (_is_increment) _parent->StepIncEvent();
          else _parent->StepDecEvent();
        }
        __super_t::on_wm_click(m);
      };

      virtual void on_wm_paint(const device_context& dc, const paint_struct&ps) override{
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

    struct value_page_button : label{

      explicit value_page_button(scrollbar_t * pParent, bool IsIncrement) : label(pParent), _parent(pParent), _is_increment(IsIncrement)
      {

      }
      virtual void on_wm_click(const mouse_msg_param& m) override{
        if (mouse_msg_param::buttons::left != m.button) return label::on_wm_click(m);
        if (_is_increment) _parent->PageUpEvent();
        else _parent->PageDownEvent();
        label::on_wm_click(m);
      };

      scrollbar_t * _parent;
      bool _is_increment;
    };

    struct slider : window<slider, policy::isa_button>{
      using __super_t = window<slider, policy::isa_button>;
      explicit slider(scrollbar_t * pParent) : __super_t(pParent){}
      virtual void handle_msg(window_message& msg) override{}
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

  struct scrollbar : window<scrollbar, policy::isa_scrollbar>{
    explicit scrollbar(iwindow * pParent) : window(pParent){}
  };

}
