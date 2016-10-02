#pragma once

namespace wtf{


  template <typename _SuperT> struct has_text_metrics : _SuperT{
    size text_size() const{
      size oRet;
      wtf::exception::throw_lasterr_if(::GetTextExtentPoint32(*this, _text.c_str(), static_cast<int>(_text.size()), &oRet), [](BOOL b){ return !b; });
      return oRet;
    }

  };

  template <typename _SuperT> struct has_font: _SuperT{
    has_font() : _font(wtf::non_client_metrics::get().lfMenuFont){}

    virtual const wtf::font& font() const { return _font; }

  protected:
    wtf::font _font;
  };



  template <typename _SuperT> struct has_paint_event: _SuperT{

    std::function<void(const paint_struct&)> OnPaintEvent;

  protected:
    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      if (WM_PAINT == umsg && OnPaintEvent) OnPaintEvent(*reinterpret_cast<const paint_struct*>(lparam));
      return 0;
    }
  };



  template <typename _SuperT> struct has_tracking_border : _SuperT{
  protected:
    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      switch (umsg){
        case WM_PAINT:{
          if (GetCapture() != hwnd) break;
          auto p1 = pen::create(pen::style::solid, 1, system_colors::button_highlight);
          auto p2 = pen::create(pen::style::solid, 1, system_colors::button_shadow);
          if (_Down) std::swap(p1, p2);
          auto rc = rect::get_client_rect(*this);
          DC().line(p1, 0, 0, rc.right, 0);
          DC().line(p1, 0, 0, 0, rc.bottom);
          DC().line(p2, rc.right - 1, 0, rc.right - 1, rc.bottom - 1);
          DC().line(p2, 0, rc.bottom - 1, rc.right, rc.bottom - 1);
          break;
        }
        case WM_LBUTTONDOWN:{
          _Down = true;
          SetCapture(hwnd);
          InvalidateRect(hwnd, nullptr, TRUE);
          break;
        }
        case WM_LBUTTONUP:{
          _Down = false;
          ReleaseCapture();
          InvalidateRect(hwnd, nullptr, TRUE);
          break;
        }
        case WM_MOUSEMOVE:{
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

    virtual void OnClick(){}

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
        ReleaseCapture(); _Down = false; OnClick();
      }
      return 0;
    }
    bool _Down = false;
  };



  template <typename _SuperT> struct has_click_event : has_click<_SuperT>{
    std::function<void()> OnClickEvent;
  protected:
    virtual void OnClick(){ if (OnClickEvent) OnClickEvent(); }
  };

  

  template <typename _SuperT> struct has_background : _SuperT{
    virtual ~has_background() = default;
    has_background() : _SuperT(), _background_brush(brush::system_brush(system_colors::button_face)) {}

    virtual const brush& background_brush() const { return _background_brush; }

  protected:
    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      if (WM_ERASEBKGND == umsg){
          device_context oCtx(reinterpret_cast<HDC>(wparam));
          auto oRect = rect::get_client_rect(*this);
          oCtx.fill(oRect, background_brush());
          bhandled = true;
          return 1;

      }
      return 0;
    }
  private:
    brush _background_brush;
  };



  template <typename _SuperT> struct has_cursor : wm_setcursor<_SuperT>{

    virtual ~has_cursor() = default;
    has_cursor() = default;

    virtual const wtf::cursor& cursor_pointer() const{ return cursor::global(cursor::style::arrow); }

    virtual const wtf::cursor& cursor_size_ns() const{ return cursor::global(cursor::style::size_ns); }

    virtual const wtf::cursor& cursor_size_we() const { return cursor::global(cursor::style::size_we); }

    virtual const wtf::cursor& cursor_size_nwse() const {return cursor::global(cursor::style::size_nwse); }

    virtual const wtf::cursor& cursor_size_nesw() const { return cursor::global(cursor::style::size_nesw); }


  protected:
    virtual void OnSetCursor(wm_nchittest_flags flags) override{
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
            SetCursor(cursor_pointer());
            break;
        }
    }
  };


  template <typename _SuperT> struct has_border : _SuperT{

    enum class border_styles{
      none,
      raised,
      lowered,
      etched,
      bumbed,
    };

    virtual ~has_border() = default;

    has_border() : _SuperT(), _border_style(border_styles::raised),
      _border_highlight(pen::create(pen::style::solid, 1, system_colors::button_highlight)),
      _border_shadow(pen::create(pen::style::solid, 1, system_colors::button_shadow)){}

    border_styles border_style() const{ return _border_style; }
    void border_style(border_styles newval){ _border_style = newval; }

    const pen& border_highlight() const { return _border_highlight; }
    void border_highlight(pen& newval){ _border_highlight = newval; }

    const pen& border_shadow() const { return _border_shadow; }
    void border_shadow(pen& newval){ _border_shadow = newval; }

  protected:
    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      if (WM_PAINT == umsg && border_styles::none != _border_style){

        auto pri = [this]() -> const pen&{ return ((border_styles::raised == _border_style || border_styles::bumbed == _border_style) ? border_highlight() : border_shadow()); };
        auto sec = [this]() -> const pen&{ return ((border_styles::raised == _border_style || border_styles::bumbed == _border_style) ? border_shadow() : border_highlight()); };

        auto rc = rect::get_client_rect(*this);

        DC().line(pri(), 0, 0, rc.right, 0);
        DC().line(pri(), 0, 0, 0, rc.bottom);
        DC().line(sec(), rc.right - 1, 0, rc.right - 1, rc.bottom - 1);
        DC().line(sec(), 0, rc.bottom - 1, rc.right, rc.bottom - 1);

        if (border_styles::bumbed == _border_style || border_styles::etched == _border_style){
          DC().line(sec(), 2, 2, rc.right - 4, 2);
          DC().line(sec(), 2, 2, 2, rc.bottom - 4);
          DC().line(pri(), rc.right - 4, 2, rc.right - 4, rc.bottom - 4);
          DC().line(pri(), 2, rc.bottom - 4, rc.right - 4, rc.bottom - 4);
        }

      }
      return 0;
    }
  private:
    pen _border_highlight;
    pen _border_shadow;
    border_styles _border_style;
  };
  


  template <typename _SuperT> struct has_close : _SuperT{
    void close() { CloseWindow(*this); }
  };

  template <typename _SuperT> struct has_show : _SuperT{
    void show(){ ::ShowWindow(*this, SW_SHOW); }
    void hide(){ ::ShowWindow(*this, SW_HIDE); }
  };


  template <typename _SuperT> struct has_size : _SuperT{
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
  protected:
    int _top, _left, _width, _height;
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
    has_text() = default;
    virtual ~has_text() = default;

    const tstring& text() const{ return _text; }

    void text(const tstring& newval){
      _text = newval;
    }

  protected:

    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      if (WM_PAINT == umsg){
        auto pPaint = reinterpret_cast<paint_struct*>(lparam);
        HFONT hOldFont, hFont = (HFONT)GetStockObject(SYSTEM_FONT);
        SetBkMode(*this, TRANSPARENT);
        if (hOldFont = (HFONT)SelectObject(*this, hFont)){
          DrawText(*this, text().c_str(), static_cast<int>(text().size()), &pPaint->client_area(), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
          SelectObject(*this, hOldFont);
        }
        SetBkMode(*this, OPAQUE);
      }
      return 0;
    }

    tstring _text;
  };

}