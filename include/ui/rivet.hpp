#pragma once
namespace wtf{
  struct rivet : window<rivet, policy::has_paint, policy::has_size, policy::has_create>{
    rivet() = delete;
    rivet(const rivet&) = delete;
    rivet(rivet&&) = delete;
    rivet &operator=(const rivet &) = delete;
    rivet &operator=(rivet&&) = delete;
    virtual ~rivet() = default;
    explicit rivet(iwindow * pParent, int x, int y) : window(pParent),
      _region(region::create_round_rect(0,0,10,10, 9, 9)), _x(x), _y(y)
    {
      OnCreate += [this]{ move(_x, _y, 10, 10); };
      OnPaint += [this](const device_context& dc, const paint_struct&){
        _region.clip(dc);
        dc.fill(_region, brush::system_brush(system_colors::button_shadow));
        dc.arch(pen::create(pen::style::solid, 1, rgb(255, 255, 255)), false, 0, 0, 10, 10, 5, 0, 0, 5);
        dc.arch(pen::create(pen::style::solid, 1, rgb(0, 0, 0)), true, 0, 0, 9, 9, 8, 4, 4, 8);
      };

    }

  private:
    int _x, _y;
    region _region;
    
  };
}

