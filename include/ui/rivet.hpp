#pragma once
namespace wtf{
  struct rivet : window<rivet, policy::has_paint, policy::has_size>{
    rivet() = delete;
    rivet(const rivet&) = delete;
    rivet(rivet&&) = delete;
    rivet &operator=(const rivet &) = delete;
    rivet &operator=(rivet&&) = delete;
    virtual ~rivet() = default;
    explicit rivet(HWND hParent, int x, int y) 
      : window(hParent),
      _region(region::create_round_rect(0,0,10,10, 9, 9))
    {
      move(x, y, 10, 10);

    }

  private:

    virtual void PaintEvent(const device_context& dc, const paint_struct&) override{
      _region.clip(dc);
      dc.fill(_region, brush::system_brush(system_colors::button_shadow));
      dc.arch(pen::create(pen::style::solid, 1, rgb(255, 255, 255)), false, 0, 0, 10, 10, 5, 0, 0, 5);
      dc.arch(pen::create(pen::style::solid, 1, rgb(0, 0, 0)), true, 0, 0, 9, 9, 8, 4, 4, 8);
    }
    region _region;
    
  };
}