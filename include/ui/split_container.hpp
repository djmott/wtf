#pragma once
namespace wtf{
  struct split_container : window<split_container, policy::has_size, policy::has_border, policy::has_paint>{

    split_container(HWND parent) 
      : window(parent),
      _first(*this), _second(*this),
      _orientation(orientations::vertical),
      _splitter(*this)
    {
      border_style(border_styles::none);
      set_split_position(25);
      _first.border_style(panel::border_styles::raised);
      _second.border_style(panel::border_styles::raised);
    }

    enum class orientations{
      horizontal,
      vertical,
    };

    orientations orientation() const{ return _orientation; }
    void orientation(orientations newval){ _orientation; }

    void set_split_position(int percentage){
      int newPos = percentage * width() / 100;
      if (orientations::horizontal == _orientation){
        _splitter.move(0, newPos, width(), SplitterWidth);
      } else{
        _splitter.move(newPos, 0, SplitterWidth, height());
      }
      ResizedEvent(has_size::wm_size_flags::restored, point::client_coords(width(), height()));
    }
    
    panel& first(){ return _first; }
    const panel& first() const{ return _first; }

    panel& second(){ return _second; }
    const panel& second() const{ return _second; }

  private:

    static const int SplitterWidth = 5;

    panel _first, _second;

    orientations _orientation;

    virtual void ResizedEvent(wm_size_flags, const point::client_coords& p){
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
    }

    void SplitterMoved(short x, short y){
      if (orientations::horizontal == _orientation){
        _splitter.move(0, _splitter.top() + y, width(), SplitterWidth);
      } else{
        _splitter.move(_splitter.left() + x, 0, SplitterWidth, height());
      }
      ResizedEvent(wm_size_flags::restored, rect::client_coord::get(*this).dimensions());
      refresh();
    }

    struct splitter : wtf::window<splitter, policy::has_cursor, policy::has_mouse, policy::has_size, policy::has_border, policy::has_paint>{

      splitter(split_container& parent) : window(parent), _parent(parent){
        border_style(border_styles::none);
      }

      split_container& _parent;

      virtual const wtf::cursor &cursor_pointer() const override{
        if (orientations::horizontal == _parent._orientation){
          return cursor::global(cursor::style::size_ns);
        } else{
          return cursor::global(cursor::style::size_we);
        }
      }

      virtual void MouseMoveEvent(event_vkeys k, const point::client_coords& p){
        if (event_vkeys::left != k) return;
        _parent.SplitterMoved(p.x, p.y);
      }

      virtual void MouseLButtonDownEvent(event_vkeys k, const point::client_coords&p){
        SetCapture(*this);
      }

      virtual void MouseLButtonUpEvent(event_vkeys k, const point::client_coords&p){
        ReleaseCapture();
      }

    }_splitter;

  };
}
