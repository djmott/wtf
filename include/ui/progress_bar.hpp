#pragma once

namespace wtf{

  struct progress_bar : wtf::window < progress_bar, policy::has_border, policy::has_size, 
    policy::has_text, policy::has_font, policy::has_paint, policy::has_orientation>{

    explicit progress_bar(HWND hParent) : window(hParent, true){}

    int min() const{ return _min; }
    void min(int newval){ _min = newval; }

    int max() const{ return _max; }
    void max(int newval){ _max = newval; }

    int value() const{ return _value; }
    void value(int newval){ _value = newval; refresh(); }

    rgb fill_color() const{ return _fill_color; }
    void fill_color(rgb newval){ _fill_color = newval; }


  protected:

    int _min = 0;
    int _max = 100;
    int _value = 0;
    rgb _fill_color = system_rgb<system_colors::highlight>();

    virtual void PaintEvent(const device_context& dc, const paint_struct& ps){
      auto oBrush = brush::solid_brush(_fill_color);
      rect::client_coord oFillArea = ps.client();
      if (orientations::horizontal == orientation()){

      } else{

      }
      dc.fill(oFillArea, oBrush);
    }


  };

}
