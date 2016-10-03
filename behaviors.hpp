#pragma once

namespace wtf{
  enum class alignment{
    Near,
    Center,
    Far,
  };

  template <typename _SuperT> struct has_timer : _SuperT{
    has_timer() : _next_timer_id(1){}

    callback<void(UINT_PTR)> TimerEvent;

    UINT_PTR set_timer(UINT elapse){
      _next_timer_id++;
      return wtf::exception::throw_lasterr_if(::SetTimer(*this, _next_timer_id, elapse, nullptr), [](UINT_PTR x){ return !x; });
    }

    void kill_timer(UINT_PTR timer_id){
      wtf::exception::throw_lasterr_if(::KillTimer(*this, timer_id), [](UINT_PTR x){ return !x; });
    }

  protected:
    UINT_PTR _next_timer_id;
    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      if (WM_TIMER == umsg) TimerEvent(static_cast<UINT_PTR>(wparam));
      return 0;
    }
  };


  template <typename _SuperT> struct has_paint : _SuperT{
    wtf::callback<void(const device_context&, const paint_struct&)> PaintEvent;

    void update() const{
      wtf::exception::throw_lasterr_if(::UpdateWindow(*this), [](BOOL b){ return !b; });
    }

    void refresh(bool erase = true) const{
      auto area = rect::get_client_rect(*this);
      wtf::exception::throw_lasterr_if(::InvalidateRect(*this, &area, erase ? TRUE : FALSE), [](BOOL b){ return !b; });
    }

  protected:
    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      if (WM_PAINT == umsg){
        PaintEvent(*reinterpret_cast<const device_context*>(wparam), *reinterpret_cast<const paint_struct*>(lparam));
      }
      return 0;
    }
  };

  template <typename _SuperT> struct has_tracking_border : _SuperT{
  protected:
    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      switch (umsg){
        case WM_PAINT:
        {
          if (GetCapture() != hwnd) break;
          auto & oDC = *reinterpret_cast<const device_context*>(wparam);
          auto p1 = pen::create(pen::style::solid, 1, system_colors::button_highlight);
          auto p2 = pen::create(pen::style::solid, 1, system_colors::button_shadow);
          if (_Down) std::swap(p1, p2);
          auto rc = rect::get_client_rect(*this);
          oDC.line(p1, 0, 0, rc.right, 0);
          oDC.line(p1, 0, 0, 0, rc.bottom);
          oDC.line(p2, rc.right - 1, 0, rc.right - 1, rc.bottom - 1);
          oDC.line(p2, 0, rc.bottom - 1, rc.right, rc.bottom - 1);
          break;
        }
        case WM_LBUTTONDOWN:
        {
          _Down = true;
          SetCapture(hwnd);
          InvalidateRect(hwnd, nullptr, TRUE);
          break;
        }
        case WM_LBUTTONUP:
        {
          _Down = false;
          ReleaseCapture();
          InvalidateRect(hwnd, nullptr, TRUE);
          break;
        }
        case WM_MOUSEMOVE:
        {
          bool bShow = true;
          auto oRect = rect::get_client_rect(*this);
          SetCapture(hwnd);
          auto x = LOWORD(lparam);
          auto y = HIWORD(lparam);
          if (x<0 || y<0 || x>oRect.right || y>oRect.bottom){
            _Down = bShow = false; ReleaseCapture();
          }
          if (bShow != _ShowBorder){
            _ShowBorder = bShow;
            InvalidateRect(hwnd, nullptr, TRUE);
          }
          break;
        }
      }
      return 0;
    }
    bool _Down = false;
    bool _ShowBorder = false;
  };

  template <typename _SuperT> struct has_click : _SuperT{
    virtual ~has_click() = default;

    wtf::callback<void(const point&)> ClickEvent;

  protected:
    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      if (WM_LBUTTONDOWN == umsg){
        _Down = true;
        SetCapture(hwnd);
      }
      if (WM_MOUSEMOVE == umsg && _Down){
        auto oRect = rect::get_client_rect(hwnd);
        auto x = LOWORD(lparam);
        auto y = HIWORD(lparam);
        if (x<0 || y<0 || x>oRect.right || y>oRect.bottom){
          _Down = false; ReleaseCapture();
        }
      }
      if (WM_LBUTTONUP == umsg && _Down){
        ReleaseCapture(); _Down = false; ClickEvent(point(LOWORD(lparam), HIWORD(lparam)));
      }
      return 0;
    }
    bool _Down = false;
  };

  template <typename _SuperT> struct has_focus : _SuperT{
    void set_focus() const{ ::SetFocus(*this); }
    bool current_focus() const{ return::GetFocus() == _SuperT::_handle; }
  protected:
    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      if (WM_LBUTTONDOWN == umsg) set_focus();
      return 0;
    }
  };

  template <typename _SuperT> struct has_caret : _SuperT{
    void create_caret() const{
      wtf::exception::throw_lasterr_if(::CreateCaret(*this, nullptr, 1, 15), [](BOOL b){ return !b; });
    }
    void destroy_caret() const{
      wtf::exception::throw_lasterr_if(::DestroyCaret(), [](BOOL b){ return !b; });
    }
    void show_caret() const{
      wtf::exception::throw_lasterr_if(::ShowCaret(*this), [](BOOL b){ return !b; });
    }
    void hide_caret() const{
      wtf::exception::throw_lasterr_if(::HideCaret(*this), [](BOOL b){ return !b; });
    }
    UINT caret_blink_time() const{
      return wtf::exception::throw_lasterr_if(::GetCaretBlinkTime(), [](UINT i){ return !i; });
    }
    void caret_blink_time(UINT newval) const{
      wtf::exception::throw_lasterr_if(::SetCaretBlinkTime(newval), [](BOOL b){ return !b; });
    }
    void set_caret_pos(const point& pos) const{
      wtf::exception::throw_lasterr_if(::SetCaretPos(pos.x, pos.y), [](BOOL b){ return !b; });
    }
  protected:
    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      if (WM_SETFOCUS == umsg){
        std::cout << "make caret" << std::endl;
        create_caret();
        set_caret_pos(point(1, 1));
        show_caret();
        caret_blink_time(250);
      } else if (WM_KILLFOCUS == umsg){
        std::cout << "destroy caret" << std::endl;
        destroy_caret();
      }
      return 0;
    }
  };

  template <typename _SuperT> struct has_background : _SuperT{
    virtual ~has_background() = default;

    has_background() : _SuperT(), _background_brush(brush::system_brush(system_colors::button_face)){}

    virtual const brush& background_brush() const{ return _background_brush; }

    void background_brush(brush&& newval){ _background_brush.swap(newval); }

  protected:
    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      if (WM_ERASEBKGND == umsg){
        auto & oDC = *reinterpret_cast<const device_context*>(lparam);
        auto oRect = rect::get_client_rect(*this);
        oDC.fill(oRect, background_brush());
        bhandled = true;
        return 1;
      }
      return 0;
    }
  private:
    brush _background_brush;
  };

  enum class wm_nchittest_flags{
    error = HTERROR,
    transparent = HTTRANSPARENT,
    nowhere = HTNOWHERE,
    client = HTCLIENT,
    caption = HTCAPTION,
    sysmenu = HTSYSMENU,
    growbox = HTGROWBOX,
    size = HTSIZE,
    menu = HTMENU,
    hscroll = HTHSCROLL,
    vscroll = HTVSCROLL,
    minbutton = HTMINBUTTON,
    maxbutton = HTMAXBUTTON,
    left = HTLEFT,
    right = HTRIGHT,
    top = HTTOP,
    topleft = HTTOPLEFT,
    topright = HTTOPRIGHT,
    bottom = HTBOTTOM,
    bottomleft = HTBOTTOMLEFT,
    bottomright = HTBOTTOMRIGHT,
    border = HTBORDER,
    reduce = HTREDUCE,
    zoom = HTZOOM,
    sizefirst = HTSIZEFIRST,
    sizelast = HTSIZELAST,
    object = HTOBJECT,
    close = HTCLOSE,
    help = HTHELP,
  };

  template <typename _SuperT> struct has_cursor : _SuperT{
    virtual ~has_cursor() = default;
    has_cursor() = default;

    virtual const wtf::cursor& cursor_pointer() const{ return cursor::global(cursor::style::arrow); }

    virtual const wtf::cursor& cursor_size_ns() const{ return cursor::global(cursor::style::size_ns); }

    virtual const wtf::cursor& cursor_size_we() const{ return cursor::global(cursor::style::size_we); }

    virtual const wtf::cursor& cursor_size_nwse() const{ return cursor::global(cursor::style::size_nwse); }

    virtual const wtf::cursor& cursor_size_nesw() const{ return cursor::global(cursor::style::size_nesw); }

  protected:

    virtual void OnSetCursor(wm_nchittest_flags flags){
      switch (flags){
        case wm_nchittest_flags::top:
        case wm_nchittest_flags::bottom:
          SetCursor(cursor_size_ns()); break;
        case wm_nchittest_flags::bottomleft:
        case wm_nchittest_flags::topright:
          SetCursor(cursor_size_nesw()); break;
        case wm_nchittest_flags::bottomright:
        case wm_nchittest_flags::topleft:
          SetCursor(cursor_size_nwse()); break;
        case wm_nchittest_flags::left:
        case wm_nchittest_flags::right:
          SetCursor(cursor_size_we()); break;
        default:
          SetCursor(cursor_pointer()); break;
      }
    }

    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      if (WM_SETCURSOR == umsg){
        OnSetCursor(static_cast<wm_nchittest_flags>(LOWORD(lparam)));
        bhandled = true;
        return TRUE;
      }
      return 0;
    }
  };

  template <typename _SuperT> struct has_border : _SuperT{
    using border_edges = device_context::border_edges;
    using border_flags = device_context::border_flags;

    virtual ~has_border() = default;

    has_border() : _SuperT(), _border_edge(border_edges::none){
      int i = (border_flags::adjust | border_flags::whole_rect | border_flags::soft | border_flags::flat);
      _border_flag = static_cast<border_flags>(i);
    }

    border_edges border_edge() const{ return _border_edge; }
    void border_edge(border_edges newval){ _border_edge = newval; }

    border_flags border_flag() const{ return _border_flag; }
    void border_flag(border_flags newval){ _border_flag = newval; }
  protected:
    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      if (WM_PAINT == umsg && border_edges::none != _border_edge){
        auto & oDC = *reinterpret_cast<const device_context*>(wparam);
        auto & pPS = *reinterpret_cast<const paint_struct*>(lparam);
        oDC.draw_edge(pPS.client_area(), _border_edge, _border_flag);
      }
      return 0;
    }

  private:
    border_edges _border_edge;
    border_flags _border_flag;
  };

  template <typename _SuperT> struct has_button_border : _SuperT{
    has_button_border() : _SuperT(), _border_edge(border_edges::thin_raised){}
  protected:
    using border_edges = device_context::border_edges;
    using border_flags = device_context::border_flags;

    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      auto iOriginalEdge = _border_edge;
      if (WM_LBUTTONDOWN == umsg){
        _border_edge = border_edges::thin_sunken;
      } else if (WM_LBUTTONUP == umsg){
        _border_edge = border_edges::thin_raised;
      } else if (WM_MOUSEMOVE == umsg){
        int x = LOWORD(lparam);
        int y = HIWORD(lparam);
        auto client = rect::get_client_rect(hwnd);
        if (x<0 || y<0 || x>client.right || y>client.bottom){
          _border_edge = border_edges::thin_raised;
        }
      } else if (WM_PAINT == umsg){
        auto & oDC = *reinterpret_cast<const device_context*>(wparam);
        auto & pPS = *reinterpret_cast<const paint_struct*>(lparam);
        static int _border_flag = (border_flags::adjust | border_flags::whole_rect);

        oDC.draw_edge(pPS.client_area(), _border_edge, static_cast<border_flags>(_border_flag));
      }
      if (iOriginalEdge != _border_edge){
        auto client = rect::get_client_rect(hwnd);
        InvalidateRect(hwnd, &client, TRUE);
      }
      return 0;
    }
    bool _Down = false;
    border_edges _border_edge;
  };

  template <typename _SuperT> struct has_close : _SuperT{
    void close(){ CloseWindow(*this); }
    callback<void()> CloseEvent;
  protected:
    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      if (WM_CLOSE == umsg) CloseEvent();
      return 0;
    }
  };

  template <typename _SuperT> struct has_show : _SuperT{
    void show(){ ::ShowWindow(*this, SW_SHOW); }
    void hide(){ ::ShowWindow(*this, SW_HIDE); }

    enum class visibility_change_flag{
      show_window = 0,
      other_unzoom = SW_OTHERUNZOOM,
      other_zoom = SW_OTHERZOOM,
      parent_closing = SW_PARENTCLOSING,
      parent_opening = SW_PARENTOPENING,
    };
    callback<void(visibility_change_flag)> VisibilityChangedEvent;

  protected:
    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      if (WM_SHOWWINDOW == umsg) VisibilityChangedEvent(static_cast<visibility_change_flag>(lparam));
      return 0;
    }
  };

  enum event_vkeys{
    control = MK_CONTROL,
    left = MK_LBUTTON,
    middle = MK_MBUTTON,
    right = MK_RBUTTON,
    shift = MK_SHIFT,
    xbutton1 = MK_XBUTTON1,
    xbutton2 = MK_XBUTTON2,
  };

  template <typename _SuperT> struct has_mouse_events : _SuperT{
    callback<void(event_vkeys, const point&)> MouseMoveEvent;
    callback<void(event_vkeys, const point&)> MouseLButtonDownEvent;
    callback<void(event_vkeys, const point&)> MouseLButtonUpEvent;
    callback<void(event_vkeys, short delta, const point&)> MouseWheelEvent;
  protected:
    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      if (WM_MOUSEMOVE == umsg) MouseMoveEvent(static_cast<event_vkeys>(wparam), point(LOWORD(lparam), HIWORD(lparam)));
      if (WM_LBUTTONDOWN == umsg) MouseLButtonDownEvent(static_cast<event_vkeys>(wparam), point(LOWORD(lparam), HIWORD(lparam)));
      if (WM_LBUTTONUP == umsg) MouseLButtonUpEvent(static_cast<event_vkeys>(wparam), point(LOWORD(lparam), HIWORD(lparam)));
      if (WM_MOUSEWHEEL == umsg) MouseWheelEvent(static_cast<event_vkeys>(LOWORD(wparam)), static_cast<short>(HIWORD(wparam)), point(LOWORD(lparam), HIWORD(lparam)));
      return 0;
    }
  };

  template <typename _SuperT> struct has_size : _SuperT{
    enum class wm_size_flags{
      hide = SIZE_MAXHIDE,
      maximized = SIZE_MAXIMIZED,
      show = SIZE_MAXSHOW,
      minimized = SIZE_MINIMIZED,
      restored = SIZE_RESTORED,
    };

    void move(int x, int y, int width, int height, bool repaint = true){
      _left = x;
      _top = y;
      _width = width;
      _height = height;
      wtf::exception::throw_lasterr_if(::MoveWindow(*this, x, y, width, height, repaint ? TRUE : FALSE), [](BOOL b){return !b; });
    }
    int top() const{ return _top; }
    int left() const{ return _left; }
    int width() const{ return _width; }
    int height() const{ return _height; }
    has_size() : _SuperT(), _top(CW_USEDEFAULT), _left(CW_USEDEFAULT), _width(CW_USEDEFAULT), _height(CW_USEDEFAULT){}

    callback<void(const rect&)> MovingEvent;
    callback<void(const point&)> MovedEvent;
    callback<void(wm_size_flags, int /*width*/, int /*height*/)> ResizedEvent;

  protected:
    int _top, _left, _width, _height;
    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      if (WM_MOVING == umsg) MovingEvent(*reinterpret_cast<const rect*>(lparam));
      if (WM_MOVE == umsg) MovedEvent(point(LOWORD(lparam), HIWORD(lparam)));
      if (WM_SIZE == umsg) ResizedEvent(static_cast<wm_size_flags>(wparam), LOWORD(lparam), HIWORD(lparam));
      return 0;
    }
  };

  template <typename _SuperT> struct has_titlebar : _SuperT{
    has_titlebar() = default;
    virtual ~has_titlebar() = default;

    const tstring& titlebar() const{
      return _titlebar;
    }

    void titlebar(LPCTSTR newval){
      wtf::exception::throw_lasterr_if(::SetWindowText(*this, newval), [](BOOL b){return !b; });
      _titlebar = newval;
    }

  protected:

    tstring _titlebar;
  };

  template <typename _SuperT> struct has_text : _SuperT{
    using draw_text_flags = device_context::draw_text_flags;
    using background_mix_modes = device_context::background_mix_modes;

    
    
    has_text()
      : _SuperT()
      , _font(wtf::non_client_metrics::get().lfMessageFont), _text(_T("")),
      _text_flags(static_cast<draw_text_flags>(draw_text_flags::center | draw_text_flags::vcenter)),
      _vertical_alignment(alignment::Center), _horizontal_alignment(alignment::Center){}
    virtual ~has_text() = default;

    const tstring& text() const{ return _text; }

    void text(const tstring& newval){
      _text = newval;
    }

    virtual const wtf::font& font() const{ return _font; }


    alignment horizontal_alignment() const{ return _horizontal_alignment; }
    void horizontal_alignment(alignment newval){ _horizontal_alignment = newval; }

    alignment vertical_alignment() const{ return _vertical_alignment; }
    void vertical_alignment(alignment newval){ _vertical_alignment = newval; }

  protected:

    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      if (WM_PAINT == umsg){
        auto pPaint = reinterpret_cast<paint_struct*>(lparam);
        auto & oDC = *reinterpret_cast<const device_context*>(wparam);
        auto iPrevBackgroundMode = oDC.background_mix_mode();
        oDC.background_mix_mode(background_mix_modes::transparent);
        auto oClient = rect::get_client_rect(*this);
        oDC.draw_text(_text, oClient, _text_flags);
        oDC.background_mix_mode(iPrevBackgroundMode);
      }
      return 0;
    }

    wtf::font _font;
    tstring _text;
    alignment _horizontal_alignment;
    alignment _vertical_alignment;
    draw_text_flags _text_flags;
  };
}
