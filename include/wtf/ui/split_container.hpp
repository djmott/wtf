#pragma once


namespace wtf{
  namespace policy{
    template <typename _SuperT, typename _ImplT>
    class isa_split_container : public _SuperT{
    public:

      explicit isa_split_container(iwindow * pParent) : _SuperT(pParent), _first(this), _second(this), _splitter(this){}


      void set_split_position(int percentage){
	      int newPos = percentage * _SuperT::width() / 100;
	      if (orientations::horizontal == _SuperT::orientation()) {
		      _splitter.move(0, newPos, _SuperT::width(), SplitterWidth);
        } else{
	        _splitter.move(newPos, 0, SplitterWidth, _SuperT::height());
        }
	      _SuperT::invalidate();
      }

      panel * first(){ return &_first; }
      const panel * first() const{ return &_first; }

      panel * second(){ return &_second; }
      const panel * second() const{ return &_second; }

    protected:

      void on_wm_create() override{
	      _SuperT::border_style(border_styles::none);
        set_split_position(25);
        _first.border_style(border_styles::raised);
        _second.border_style(border_styles::raised);
        _SuperT::on_wm_create();
      };

      void on_wm_size(const point<coord_frame::client>& p) override{
	      if (orientations::horizontal == _SuperT::orientation()) {
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
        _SuperT::on_wm_size(p);
      };

      static const int SplitterWidth = 5;

      panel _first, _second;

      void size_bar_moved(const point<coord_frame::client>& p){
	      if (orientations::horizontal == _SuperT::orientation()) {
		      _splitter.move(0, _splitter.top() + p.y, _SuperT::width(), SplitterWidth);
        } else{
	        _splitter.move(_splitter.left() + p.x, 0, SplitterWidth, _SuperT::height());
        }
	      _SuperT::invalidate();
      }


      class size_bar : public window_impl<size_bar, policy::wm_create, policy::has_cursor, policy::wm_mouse_move, policy::isa_panel>{
	      using __super_t = window_impl<size_bar, policy::wm_create, policy::has_cursor, policy::wm_mouse_move, policy::isa_panel>;
      public:
	      size_bar(isa_split_container * pParent)
		      : __super_t(pParent)
		      , _parent(pParent) {}


        void on_wm_create() override{
	        __super_t::border_style(border_styles::none);
	        __super_t::on_wm_create();
        };

        void on_wm_mouse_move(const mouse_msg_param& m) override{
	        if (!(m.key_state & mouse_msg_param::key_states::left)) return __super_t::on_wm_mouse_move(m);
          _parent->size_bar_moved(m.position);
	        __super_t::on_wm_mouse_move(m);
        };

        void on_wm_mouse_down(const mouse_msg_param& m) override{
	        if (mouse_msg_param::buttons::left != m.button) return __super_t::on_wm_mouse_down(m);
          SetCapture(*this);
	        __super_t::on_wm_mouse_down(m);
        };

        void on_wm_mouse_up(const mouse_msg_param& m) override{
          ReleaseCapture();
	        __super_t::on_wm_mouse_up(m);
        };

        isa_split_container * _parent;

        const wtf::cursor &cursor_pointer() const override{
          if (orientations::horizontal == _parent->orientation()){
            return cursor::global(cursor::style::size_ns);
          } else{
            return cursor::global(cursor::style::size_we);
          }
        }

      }_splitter;

    };
  }

  template <> struct policy_traits<policy::isa_split_container>{
    using requires = policy_list<policy::has_orientation, policy::wm_create, policy::isa_panel>;
  };


  struct split_container : window_impl<split_container, policy::isa_split_container>{
    explicit split_container(iwindow * pParent) : window_impl(pParent){}
  };

}
