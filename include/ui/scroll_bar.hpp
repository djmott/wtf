#pragma once
namespace wtf{


    struct scroll_bar : window<scroll_bar, policy::has_border, policy::has_size, policy::has_paint>
    {
      

      virtual ~scroll_bar() = default;
      scroll_bar(const scroll_bar&) = delete;
      scroll_bar &operator=(const scroll_bar &) = delete;
      scroll_bar(scroll_bar&&) = delete;
      scroll_bar &operator=(scroll_bar&&) = delete;

      explicit scroll_bar(HWND pParent)
        : window(pParent, true),
        increment_button(*this), 
        decrement_button(*this), 
        slider(*this), 
        _background_brush(brush::system_brush(system_colors::gray_text)), 
        _Outline(pen::create(pen::style::solid, 1, system_colors::button_text)),
        _Fill(brush::system_brush(system_colors::button_text)),
        _orientation(orientations::horizontal)
      {
        increment_button.PaintEvent.connect([this](const device_context& dc, const paint_struct& ){
          auto client = rect::get_client_rect(*this);
          point::vector arrow(3);
          if (orientations::horizontal == _orientation){
            arrow[0].x = 5; arrow[0].y = 5;
            arrow[1].x = client.right - 5; arrow[1].y = client.bottom / 2;
            arrow[2].x = 5; arrow[2].y = client.bottom - 5;
          } else{
            arrow[0].x = 5; arrow[0].y = 5;
            arrow[1].x = client.right - 5; arrow[1].y = 5;
            arrow[2].x = client.right / 2; arrow[2].y = client.bottom - 5;
          }
          dc.fill(arrow, _Outline, _Fill);
        });
        decrement_button.PaintEvent.connect([this](const device_context& dc, const paint_struct& ){
          auto client = rect::get_client_rect(*this);
          point::vector arrow(3);
          if (orientations::horizontal == _orientation){
            arrow[0].x = 5; arrow[0].y = client.bottom / 2;
            arrow[1].x = client.right - 5; arrow[1].y = 5;
            arrow[2].x = client.right - 5; arrow[2].y = client.bottom - 5;
          } else{
            arrow[0].x = client.right / 2; arrow[0].y = 5;
            arrow[1].x = client.right - 5; arrow[1].y = client.bottom - 5;
            arrow[2].x = 5; arrow[2].y = client.bottom - 5;
          }
          dc.fill(arrow, _Outline, _Fill);
        });
          
        window::ResizedEvent.connect([this](typename window::wm_size_flags, int width, int height) {
          if (orientations::horizontal == _orientation){
            decrement_button.move(0, 0, height, height);
            increment_button.move(width - height, 0, height, height);
          } else{
            decrement_button.move(0, 0, width, width);
            increment_button.move(0, height - width, width, width);
          }
        });
          
      }

      enum class orientations{
        horizontal,
        vertical
      };

      orientations orientation() const{ return _orientation; }
      void orientation(orientations newval){ _orientation = newval; }


      push_button increment_button;
      push_button decrement_button;
      push_button slider;

    protected:

      
      virtual const brush& background_brush() const override{ return _background_brush; }

      brush _background_brush;

      pen _Outline;
      brush _Fill;
      orientations _orientation;

    };

}
