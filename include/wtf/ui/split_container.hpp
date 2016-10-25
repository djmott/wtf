#pragma once

#if 0
namespace wtf{
  struct split_container : window<split_container, iwindow, 
    policy::has_size, policy::has_border, policy::has_orientation, policy::has_move, policy::has_background,
    wm_size, wm_create, wm_erasebkgnd, wm_nccalcsize, wm_ncpaint>{

    using mouse_msg_param = mouse_msg_param;

    explicit split_container(iwindow * pParent) : window(pParent), _first(this), _second(this), _splitter(this)
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

    virtual LRESULT on_wm_create(bool& bHandled) override{
      border_style(border_styles::none);
      set_split_position(25);
      _first.border_style(border_styles::raised);
      _second.border_style(border_styles::raised);
      return window::on_wm_create(bHandled);
    };

    virtual LRESULT on_wm_size(const point<coord_frame::client>& p, bool& bHandled) override{
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
      return window::on_wm_size(p, bHandled);
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


    struct size_bar : label{

      size_bar(split_container * pParent) : label(pParent), _parent(pParent){}


      virtual LRESULT on_wm_create(bool& bHandled) override{ 
        border_style(border_styles::none);
        return label::on_wm_create(bHandled);      
      };
      
      virtual LRESULT on_wm_mouse_move(const mouse_msg_param& m, bool& bHandled) override{
        if (!(m.key_state & mouse_msg_param::key_states::left)) return label::on_wm_mouse_move(m, bHandled);
        _parent->size_bar_moved(m.position);
        return label::on_wm_mouse_move(m, bHandled);
      };
      
      virtual LRESULT on_wm_mouse_down(const mouse_msg_param& m, bool& bHandled) override{
        if (mouse_msg_param::buttons::left != m.button) return label::on_wm_mouse_down(m, bHandled);
        SetCapture(*this);
        return label::on_wm_mouse_down(m, bHandled);
      };
      
      virtual LRESULT on_wm_mouse_up(const mouse_msg_param& m, bool& bHandled) override{
        ReleaseCapture();
        return label::on_wm_mouse_up(m, bHandled);
      };

      split_container * _parent;

      virtual const wtf::cursor &cursor_pointer() const override{
        if (orientations::horizontal == _parent->_orientation){
          return cursor::global(cursor::style::size_ns);
        } else{
          return cursor::global(cursor::style::size_we);
        }
      }

    }_splitter;

  };
}
#endif