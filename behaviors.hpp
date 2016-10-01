#pragma once

namespace wtf{



  
  template <typename _SuperT> struct has_background : _SuperT{
    virtual ~has_background() = default;
    has_background() : _SuperT(), _BackgroundBrush(wtf::solid_brush<RGB(0,0,0)>()) {}

    const const_brush& BackgroundBrush() const{ return *_BackgroundBrush; }
    void BackgroundBrush(const const_brush& newval){ }

  protected:
    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      if (WM_ERASEBKGND == umsg){
          device_context oCtx(reinterpret_cast<HDC>(wparam));
          auto oRect = rect::get_client_rect(hwnd);
          oCtx.fill(oRect, _BackgroundBrush);
          bhandled = true;
          return 1;

      }
      return 0;
    }
  private:
    const_brush _BackgroundBrush;
  };

  template <typename _SuperT> struct has_cursor : _SuperT{

    virtual ~has_cursor() = default;
    has_cursor() : _SuperT(), _cursor(cursors::arrow()){}

    const wtf::cursor& cursor() const{ return _cursor; }
    void cursor(const wtf::cursor& newval){ 
      _cursor = newval; 
      //SetWindowLongPtr(*this, GWL_HCURSOR, _cursor.native_handle());
//       SetClassLongPtr(*this, GCLP_HCURSOR, _cursor.native_handle());
    }
  protected:

    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      if (WM_SETCURSOR == umsg){
//         auto sTemp = std::string("WM_SETCURSOR ") + std::to_string(lparam) + "\n";
        //std::cout << "WM_SETCURSOR " << sTemp << std::endl; 
//         OutputDebugStringA(sTemp.c_str());
      }
      return 0;
    }
  protected:
    wtf::cursor _cursor;
  };

/*

  template <typename _SuperT> struct has_border : _SuperT{
    virtual ~has_border() = default;
    has_border() : _SuperT(), _Highlight(Gdiplus::Color(GetSysColor(COLOR_BTNHIGHLIGHT))), _Shadow(Gdiplus::Color(GetSysColor(COLOR_BTNSHADOW))){}

    virtual const pen& BorderHighlight() const{ return _Highlight; }
    virtual const pen& BorderShadow() const{ return _Shadow; }


    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
      __F(__FUNCTION__"\n");
      if (WM_PAINT == umsg){
        PAINTSTRUCT paint;
        BeginPaint(hwnd, &paint);

        EndPaint(hwnd, &paint);
      }
      return 0;
    }
  private:
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
*/



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