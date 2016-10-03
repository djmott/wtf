/** @file
 *
 */
#pragma once

namespace wtf{

  enum class alignment{
    Near,
    Center,
    Far,
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

  enum class event_vkeys{
    control = MK_CONTROL,
    left = MK_LBUTTON,
    middle = MK_MBUTTON,
    right = MK_RBUTTON,
    shift = MK_SHIFT,
    xbutton1 = MK_XBUTTON1,
    xbutton2 = MK_XBUTTON2,
  };

  /** has_border
   * Creates borders
   */
  template <typename _SuperT> struct has_border : _SuperT{
    using border_edges = device_context::border_edges;
    using border_flags = device_context::border_flags;

    virtual ~has_border() = default;

    has_border() : _SuperT(), _border_edge(border_edges::raised),
      _border_flag(weak_enum_class<border_flags>::set_flags(border_flags::adjust, border_flags::whole_rect, border_flags::soft, border_flags::flat, border_flags::mono))
    {}

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

  /** has_button_border
   * A clickable border that changes state with mouse/keyboard input
   */
  template <typename _SuperT> struct has_button_border : _SuperT{

    has_button_border() : _SuperT(), 
      _border_edge(border_edges::thin_raised), 
      _button_type(button_types::push),
      _button_state(button_states::up){}

    enum class button_types{
      push,
      toggle,
    };

    enum class button_states{
      up,
      down,
    };

    button_types button_type() const{ return _button_type; }
    void button_type(button_types newval){ _button_type = newval; }

    button_states button_state() const{ return _button_state; }
    void button_state(button_states newval){ _button_state = newval; }

  protected:
    using border_edges = device_context::border_edges;
    using border_flags = device_context::border_flags;

    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      auto iOriginalEdge = _border_edge;
      if (WM_LBUTTONDOWN == umsg){
        if (button_types::push == _button_type) _border_edge = border_edges::thin_sunken;
      } else if (WM_LBUTTONUP == umsg){
        if (button_types::push == _button_type){
          _border_edge = border_edges::thin_raised;
        } else{
          if (button_states::down == _button_state){
            _button_state = button_states::up;
            _border_edge = border_edges::thin_raised;
          } else{
            _button_state = button_states::down;
            _border_edge = border_edges::thin_sunken;
          }
        }
      } else if (WM_MOUSEMOVE == umsg){
        int x = LOWORD(lparam);
        int y = HIWORD(lparam);
        auto client = rect::get_client_rect(hwnd);
        if (x<0 || y<0 || x>client.right || y>client.bottom){
          if (button_types::push == _button_type) _border_edge = border_edges::thin_raised;
        }
      } else if (WM_PAINT == umsg){
        auto & oDC = *reinterpret_cast<const device_context*>(wparam);
        auto & pPS = *reinterpret_cast<const paint_struct*>(lparam);
        auto iBorderFlags = weak_enum_class<border_flags>::set_flags(border_flags::adjust, border_flags::whole_rect);

        oDC.draw_edge(pPS.client_area(), _border_edge, iBorderFlags);
      }
      if (iOriginalEdge != _border_edge){
        auto client = rect::get_client_rect(hwnd);
        InvalidateRect(hwnd, &client, TRUE);
      }
      return 0;
    }
    bool _Down = false;
    border_edges _border_edge;
    button_types _button_type;
    button_states _button_state;
  };

  /** has_caret
   * Controls the caret of text/input elements
   */
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

  /** has_click
   * produces click and double click events
   */
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

  /** has_close
   * Add members to close a UI element and events to capture it
   */
  template <typename _SuperT> struct has_close : _SuperT{
    void close(){ CloseWindow(*this); }
    callback<void()> CloseEvent;
  protected:
    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      if (WM_CLOSE == umsg) CloseEvent();
      return 0;
    }
  };

  /** has_cursor
   * Provides mouse pointer customization
   */
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

  /** has_focus
   * Provides members and events related to keyboard/mouse focus
   */
  template <typename _SuperT> struct has_focus : _SuperT {
    void set_focus() const { wtf::exception::throw_lasterr_if(::SetFocus(*this), [](HWND h){ return !h;}); }
    bool got_focus() const { return _SuperT::_handle == ::GetFocus(); }
    callback<void()> GotFocusEvent;
    callback<void()> LostFocusEvent;
  protected:
    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override {
      if (WM_SETFOCUS == umsg) GotFocusEvent();
      else if (WM_KILLFOCUS == umsg) LostFocusEvent();
      return 0;
    }
  };

  /** has_icon
   * Provides an icon associated with the window
   */
  template <typename _SuperT> struct has_icon : _SuperT{
    has_icon() : _SuperT(), _big_icon(icon::system(icon::style::shield)), 
      _small_icon(icon::system(icon::style::shield)){}

    virtual icon& big_icon(){ return _big_icon; }
    virtual const icon& big_icon() const { return _big_icon; }
    void big_icon(icon&& src){ _big_icon = std::move(src); }

    virtual icon& small_icon(){ return _small_icon; }
    virtual const icon& small_icon() const{ return _small_icon; }
    void small_icon(icon&& src){ _small_icon = std::move(src); }

  protected:
    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      if (WM_GETICON != umsg) return 0;
      bhandled = true;
      if (ICON_BIG == wparam) return reinterpret_cast<LRESULT>(static_cast<HICON>(big_icon()));
      else return reinterpret_cast<LRESULT>(static_cast<HICON>(small_icon()));
    }
    icon _big_icon;
    icon _small_icon;
  };

  /** has_mouse_events
   * produces events generated by mouse actions
   */
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

  /** has_paint
   * adds members and events related to painting
   */
  template <typename _SuperT> struct has_paint : _SuperT{

    has_paint() : _SuperT(), _background_brush(brush::system_brush(system_colors::button_face)){}

    wtf::callback<void(const device_context&, const paint_struct&)> PaintEvent;

    void update() const{
      wtf::exception::throw_lasterr_if(::UpdateWindow(*this), [](BOOL b){ return !b; });
    }

    void refresh(bool erase = true) const{
      auto area = rect::get_client_rect(*this);
      wtf::exception::throw_lasterr_if(::InvalidateRect(*this, &area, erase ? TRUE : FALSE), [](BOOL b){ return !b; });
    }

    virtual const brush& background_brush() const{ return _background_brush; }

    void background_brush(brush&& newval){ _background_brush.swap(newval); }

  protected:
    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      if (WM_PAINT == umsg){
        PaintEvent(*reinterpret_cast<const device_context*>(wparam), *reinterpret_cast<const paint_struct*>(lparam));
      }else if (WM_ERASEBKGND == umsg){
        auto & oDC = *reinterpret_cast<const device_context*>(lparam);
        auto oRect = rect::get_client_rect(*this);
        oDC.fill(oRect, background_brush());
        bhandled = true;
        return 1;
      }
      return 0;
    }

    brush _background_brush;

  };

  /** has_show
   * Adds members to change the display state and capture display state changes
   */
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

  /** has_size
   * Adds members to reposition/resize and events to capture them
   */
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

  /** has_titlebar
   * Adds titlebar text features to windows
   */
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

  /** has_text
   * provides members to draw text on UI elements
   */
  template <typename _SuperT> struct has_text : _SuperT{

    
    has_text() : _SuperT(),
      _font(wtf::non_client_metrics::get().lfMessageFont), 
      _text(_T("")),
      _fore_color(system_rgb<system_colors::window_text>()),
      _back_color(system_rgb<system_colors::window>()),
      _vertical_alignment(alignment::Center),
      _horizontal_alignment(alignment::Center),
      _background_mode(background_modes::transparent),
      _multiline(false)
    {}

    enum class background_modes{
      opaque,
      transparent,
    };

    background_modes background_mode() const{ return _background_mode; }
    void background_mode(background_modes newval){ _background_mode = newval; }

    virtual const wtf::font& font() const{ return _font; }
    virtual wtf::font& font() { return _font; }
    void font(wtf::font& newval) { _font = newval; }

    rgb fore_color() const{ return _fore_color; }
    void fore_color(rgb newval) { _fore_color = newval; }

    rgb back_color() const{ return _back_color; }
    void back_color(rgb newval){ _back_color = newval; }

    alignment vertial_alignment() const { return _vertical_alignment; }
    void vertial_alignment(alignment newval) { _vertical_alignment = newval; }

    alignment horizontal_alignment() const { return _horizontal_alignment; }
    void horizontal_alignment(alignment newval) { _horizontal_alignment = newval; }

    bool multiline() const{ return _multiline; }
    void multiline(bool newval){ _multiline = newval; }

    const tstring& text() const{ return _text; }

    void text(const tstring& newval){ _text = newval; }

  protected:

    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      using text_align_modes = device_context::text_align_modes;
      using draw_text_flags = device_context::draw_text_flags;
      if (WM_PAINT == umsg){
        auto pPaint = reinterpret_cast<paint_struct*>(lparam);
        auto & oDC = *reinterpret_cast<const device_context*>(wparam);
        auto oHandle = _font.open();
        oDC.select_object(oHandle);
        oDC.move_to(0, 0);
        //weak_enum_class<draw_text_flags> Flags(draw_text_flags::word_ellipsis);
        weak_enum_class<draw_text_flags> Flags(draw_text_flags::word_break);
        Flags |= draw_text_flags::word_break;
        switch (_vertical_alignment){
          case alignment::Near:Flags |= draw_text_flags::top; break;
          case alignment::Center:Flags |= draw_text_flags::vcenter; break;
          case alignment::Far: Flags |= draw_text_flags::bottom; break;
        }
        switch (_horizontal_alignment){
          case alignment::Near:Flags |= draw_text_flags::left; break;
          case alignment::Center: Flags |= draw_text_flags::center; break;
          case alignment::Far: Flags |= draw_text_flags::right; break;
        }
        if (!_multiline) Flags |= draw_text_flags::single_line;
        oDC.text_align(weak_enum_class<text_align_modes>::set_flags(text_align_modes::left, text_align_modes::top, text_align_modes::no_update_cp));
        if (background_modes::opaque == _background_mode){
          oDC.background_mix_mode( device_context::background_mix_modes::opaque );
        }else{
          oDC.background_mix_mode( device_context::background_mix_modes::transparent );
        }
        oDC.text_color(_fore_color);
        oDC.background_color(_back_color);
        auto oClient = rect::get_client_rect(*this);
        oDC.draw_text(_text, oClient, Flags);

      }
      return 0;
    }

    wtf::font _font;
    tstring _text;
    rgb _fore_color;
    rgb _back_color;
    alignment _vertical_alignment;
    alignment _horizontal_alignment;
    background_modes _background_mode;
    bool _multiline;
  };

  /** has_timer
   * Adds timer creation and produces timer events
   */
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

}
