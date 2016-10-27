#pragma once


namespace wtf{

  template <typename _ImplT, policy..._Policies>
  class window<_ImplT, policy::isa_split_container, _Policies...>
    : public window_impl<_ImplT, _Policies..., policy::isa_panel, policy::has_orientation>
  {
    using __split_container = window<_ImplT, policy::isa_split_container, _Policies...>;
    using __super_t = window_impl<_ImplT, _Policies..., policy::isa_panel, policy::has_orientation>;
    template <typename, policy ... > friend class window_impl;

  public:

    explicit window(iwindow * pParent) : __super_t(pParent), _first(this), _second(this), _splitter(this)
    { }


    void set_split_position(int percentage){
      int newPos = percentage * width() / 100;
      if (orientations::horizontal == _orientation){
        _splitter.move(0, newPos, width(), SplitterWidth);
      } else{
        _splitter.move(newPos, 0, SplitterWidth, height());
      }
      invalidate();
    }

    panel * first(){ return &_first; }
    const panel * first() const{ return &_first; }

    panel * second(){ return &_second; }
    const panel * second() const{ return &_second; }

  protected:

    virtual void on_wm_create() override{
      border_style(border_styles::none);
      set_split_position(25);
      _first.border_style(border_styles::raised);
      _second.border_style(border_styles::raised);
      __super_t::on_wm_create();
    };

    virtual void on_wm_size(const point<coord_frame::client>& p) override{
      if (orientations::horizontal == _orientation){
        auto NewTop = _splitter.top();
        if (NewTop < 10) NewTop = 10;
        if (NewTop > p.y - 10) NewTop = p.y - 10;
        _first.move(0, 0, p.x, NewTop);
        _splitter.move(0, NewTop, p.x, SplitterWidth);
        _second.move(0, NewTop + SplitterWidth, p.x, p.y - NewTop - SplitterWidth);
      } else{
        auto NewLeft = _splitter.left();
        if (NewLeft < 10) NewLeft = 10;
        if (NewLeft > p.x - 10) NewLeft = p.x - 10;
        _first.move(0, 0, NewLeft, p.y);
        _splitter.move(NewLeft, 0, SplitterWidth, p.y);
        _second.move(NewLeft + SplitterWidth, 0, p.x - NewLeft - SplitterWidth, p.y);
      }
      __super_t::on_wm_size(p);
    };

    static const int SplitterWidth = 5;

    panel _first, _second;

    void size_bar_moved(const point<coord_frame::client>& p){
      if (orientations::horizontal == _orientation){
        _splitter.move(0, _splitter.top() + p.y, width(), SplitterWidth);
      } else{
        _splitter.move(_splitter.left() + p.x, 0, SplitterWidth, height());
      }
      invalidate();
    }


    class size_bar : public window<size_bar, policy::isa_panel, policy::has_cursor> {
      using __super_t = window<size_bar, policy::isa_panel, policy::has_cursor>;
      template <typename, policy ... > friend class window_impl;
    public:
      size_bar(__split_container * pParent) : __super_t(pParent), _parent(pParent){}


      virtual void on_wm_create() override{ 
        border_style(border_styles::none);
        __super_t::on_wm_create();
      };
      
      virtual void on_wm_mouse_move(const mouse_msg_param& m) override{
        if (!(m.key_state & mouse_msg_param::key_states::left)) return __super_t::on_wm_mouse_move(m);
        _parent->size_bar_moved(m.position);
        __super_t::on_wm_mouse_move(m);
      };
      
      virtual void on_wm_mouse_down(const mouse_msg_param& m) override{
        if (mouse_msg_param::buttons::left != m.button) return __super_t::on_wm_mouse_down(m);
        SetCapture(*this);
        __super_t::on_wm_mouse_down(m);
      };
      
      virtual void on_wm_mouse_up(const mouse_msg_param& m) override{
        ReleaseCapture();
        __super_t::on_wm_mouse_up(m);
      };

      __split_container * _parent;

      virtual const wtf::cursor &cursor_pointer() const override{
        if (orientations::horizontal == _parent->_orientation){
          return cursor::global(cursor::style::size_ns);
        } else{
          return cursor::global(cursor::style::size_we);
        }
      }

    }_splitter;

  };

  struct split_container : window<split_container, policy::isa_split_container>{
    explicit split_container(iwindow * pParent) : window(pParent){}
  };

}
