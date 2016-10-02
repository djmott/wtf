#pragma once
namespace wtf{
  namespace default_theme{
    enum class orientation{
      horizontal,
      vertical
    };



    template <orientation _Orientation>
    struct scroll_bar : window<scroll_bar<_Orientation>, has_border, wm_size, has_background>{
      
      static const orientation Orientation = _Orientation;

      scroll_bar(HWND pParent) : window(pParent, true), _Outline(pen::create(pen::style::solid, 1, system_colors::button_text)), _Fill(brush::system_brush(system_colors::button_text)),
        _Inc(*this), _Dec(*this), _Slide(*this), _background_brush(brush::system_brush(system_colors::gray_text))
      {        
        _Inc.OnPaintEvent = [this](const device_context& dc, const rect& client){
          std::vector<POINT> arrow(3);
          if (orientation::horizontal == Orientation){
            arrow[0].x = 5; arrow[0].y = 5;
            arrow[1].x = client.right - 5; arrow[1].y = client.bottom / 2;
            arrow[2].x = 5; arrow[2].y = client.bottom - 5;
          } else{
            arrow[0].x = 5; arrow[0].y = 5;
            arrow[1].x = client.right - 5; arrow[1].y = 5;
            arrow[2].x = client.right / 2; arrow[2].y = client.bottom - 5;
          }
          dc.fill(arrow, _Outline, _Fill);        
        };
        _Dec.OnPaintEvent = [this](const device_context& dc, const rect& client){
          std::vector<POINT> arrow(3);
          if (orientation::horizontal == Orientation){
            arrow[0].x = 5; arrow[0].y = client.bottom / 2;
            arrow[1].x = client.right - 5; arrow[1].y = 5;
            arrow[2].x = client.right - 5; arrow[2].y = client.bottom - 5;
          } else{
            arrow[0].x = client.right / 2; arrow[0].y = 5;
            arrow[1].x = client.right - 5; arrow[1].y = client.bottom - 5;
            arrow[2].x = 5; arrow[2].y = client.bottom - 5;
          }
          dc.fill(arrow, _Outline, _Fill);
        };
        _Inc.OnClickEvent = [this](){increment(); };
        _Dec.OnClickEvent = [this](){decrement(); };
      }

      virtual void increment(){}
      virtual void decrement(){}

    protected:

      virtual const brush& background_brush() const override{ return _background_brush; }
      
      brush _background_brush;


      virtual void OnResized(wm_size_flags, int width, int height) override{
        if (orientation::horizontal == Orientation){
          _Dec.move(0, 0, height, height);
          _Inc.move(width - height, 0, height, height);
        } else{
          _Dec.move(0, 0, width, width);
          _Inc.move(0, height - width, width, width);
        }
      }

      pen _Outline;
      brush _Fill;

      button _Inc;
      button _Dec;
      button _Slide;
    };
  }
}