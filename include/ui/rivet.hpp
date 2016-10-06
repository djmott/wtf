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
    }
    region _region;
    
  };
}