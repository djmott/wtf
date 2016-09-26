#pragma once

namespace wtf{

  template <typename _SuperT> struct has_label : _SuperT{

    virtual ~has_label() = default;
    has_label() : _label(), _gdi_label(), _Format(){
      _Format.SetAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
      _Format.SetLineAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
    }

    const std::basic_string<TCHAR>& label_string() const{ return _label; }
    void label_string(const TCHAR * newval){ _label = newval; set_gdi_label(); }
    void label_string(const std::basic_string<TCHAR>& newval){_label = newval; set_gdi_label();}


    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wpara, LPARAM lparam, bool& bhandled) override{
      __F(__FUNCTION__"\n");
      if (WM_PAINT == umsg){
        PAINTSTRUCT paint;
        BeginPaint(hwnd, &paint);
        Gdiplus::Graphics oGraphics(paint.hdc);
        Gdiplus::RectF oGdiRect(static_cast<Gdiplus::REAL>(paint.rcPaint.left), static_cast<Gdiplus::REAL>(paint.rcPaint.top), static_cast<Gdiplus::REAL>(paint.rcPaint.right), static_cast<Gdiplus::REAL>(paint.rcPaint.bottom));
        oGraphics.DrawString(_gdi_label.c_str(), -1, &this->font(), oGdiRect, &_Format, &this->font_brush());
        EndPaint(hwnd, &paint);
      }
      return 0;
    }

  private:    
    void set_gdi_label(){
      _gdi_label.clear();
      for (auto ch : _label){
        _gdi_label.push_back(ch);
      }
    }

    std::basic_string<TCHAR> _label;
    std::wstring _gdi_label;
    Gdiplus::StringFormat _Format;
  };

  template <typename _SuperT> struct has_background : _SuperT{
    virtual ~has_background() = default;
    has_background() : _SuperT(), _Background(new Gdiplus::SolidBrush(Gdiplus::Color(GetSysColor(COLOR_BTNFACE)))){}

    const Gdiplus::Brush& BackgroundBrush() const{ return *_Background; }
    void BackgroundBrush(const Gdiplus::Brush& newval){ 
      _Background = std::unique_ptr<Gdiplus::Brush>(newval.Clone()) ;
      SetClassLongPtr(*this, GCL_HBRBACKGROUND, _Background->nativeBrush());
    }


  private:
    std::unique_ptr<Gdiplus::Brush> _Background;
  };

  template <typename _SuperT> struct has_cursor : _SuperT{

    virtual ~has_cursor() = default;
    has_cursor() : _SuperT(), _cursor(cursors::arrow()){}

    const wtf::cursor& cursor() const{ return _cursor; }
    void cursor(const wtf::cursor& newval){ 
      _cursor = newval; 
      //SetWindowLongPtr(*this, GWL_HCURSOR, _cursor.native_handle());
      SetClassLongPtr(*this, GCLP_HCURSOR, _cursor.native_handle());
    }


  protected:
    wtf::cursor _cursor;
  };

  template <typename _SuperT> struct has_border : _SuperT{
    virtual ~has_border() = default;
    has_border() : _SuperT(), _Highlight(Gdiplus::Color(GetSysColor(COLOR_BTNHIGHLIGHT))), _Shadow(Gdiplus::Color(GetSysColor(COLOR_BTNSHADOW))){}

    virtual const Gdiplus::Pen& BorderHighlight() const{ return _Highlight; }
    virtual const Gdiplus::Pen& BorderShadow() const{ return _Shadow; }


    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wpara, LPARAM lparam, bool& bhandled) override{
      __F(__FUNCTION__"\n");
      if (WM_PAINT == umsg){
        PAINTSTRUCT paint;
        BeginPaint(hwnd, &paint);
        Gdiplus::Graphics oGraphics(paint.hdc);
        oGraphics.DrawLine(&BorderHighlight(), paint.rcPaint.left, paint.rcPaint.top, paint.rcPaint.right, paint.rcPaint.top);
        oGraphics.DrawLine(&BorderHighlight(), paint.rcPaint.left, paint.rcPaint.top, paint.rcPaint.left, paint.rcPaint.top);
        oGraphics.DrawLine(&BorderShadow(), paint.rcPaint.right-1, paint.rcPaint.top, paint.rcPaint.right-1, paint.rcPaint.bottom - 1);
        oGraphics.DrawLine(&BorderShadow(), paint.rcPaint.left, paint.rcPaint.bottom-1, paint.rcPaint.right, paint.rcPaint.bottom-1);

        EndPaint(hwnd, &paint);
      }
      return 0;
    }
  private:
    Gdiplus::Pen _Highlight;
    Gdiplus::Pen _Shadow;
  };
  

  template <typename _SuperT> struct has_font : _SuperT{
    virtual ~has_font() = default;
    has_font() : _SuperT(), _font(new Gdiplus::Font(Gdiplus::FontFamily::GenericSerif(), 11)), _font_brush(Gdiplus::Color(GetSysColor(COLOR_BTNTEXT))){}


    const Gdiplus::Font& font() const{ return *_font; }
    void font(const Gdiplus::Font& newval){ _font = std::unique_ptr<Gdiplus::Font>(newval.Clone()); }

    const Gdiplus::Brush& font_brush() const{ return _font_brush; }
    void font_brush(const Gdiplus::Brush& newval){ _font_brush = newval; }
  protected:
    std::unique_ptr<Gdiplus::Font> _font;
    Gdiplus::SolidBrush _font_brush;
  };



  template <typename _SuperT> struct has_close : _SuperT{
    has_close() = default;
    virtual ~has_close() = default;
    void close() { CloseWindow(*this); }
  };

  template <typename _SuperT> struct has_show : _SuperT{
    has_show() = default;
    virtual ~has_show() = default;
    void show(){ ::ShowWindow(*this, SW_SHOW); }
    void hide(){ ::ShowWindow(*this, SW_HIDE); }
  };


  template <typename _SuperT> struct has_move : _SuperT{
    has_move() = default;
    virtual ~has_move() = default;
    void move(int x, int y, int width, int height, bool repaint = true){
      wtf::exception::throw_lasterr_if(::MoveWindow(*this, x, y, width, height, repaint ? TRUE : FALSE), [](BOOL b){return !b; });
    }
  };



  template <typename _SuperT> struct has_text : _SuperT{
    has_text() = default;
    virtual ~has_text() = default;
    const std::basic_string<TCHAR>& text() const{
      return _text;
    }

    void text(LPCTSTR newval){
      wtf::exception::throw_lasterr_if(::SetWindowText(*this, newval), [](BOOL b){return !b; });
      _text = newval;
    }
  protected:
    std::basic_string<TCHAR> _text;
  };

}