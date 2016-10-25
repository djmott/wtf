#pragma once

#if 0
namespace wtf{
  struct rivet : window<rivet, iwindow, wm_paint, policy::has_move>{
    rivet() = delete;
    rivet(const rivet&) = delete;
    rivet(rivet&&) = delete;
    rivet &operator=(const rivet &) = delete;
    rivet &operator=(rivet&&) = delete;
    virtual ~rivet() = default;
    explicit rivet(iwindow * pParent) : window(pParent),
      _region(region::create_round_rect(0,0,10,10, 9, 9)){}

    
    virtual LRESULT on_wm_paint(const device_context& dc, const paint_struct& ps, bool& bHandled) override{
      _region.clip(dc);
      dc.fill(_region, brush::system_brush(system_colors::button_shadow));
      dc.arch(pen::create(pen::style::solid, 1, rgb(255, 255, 255)), false, 0, 0, 10, 10, 5, 0, 0, 5);
      dc.arch(pen::create(pen::style::solid, 1, rgb(0, 0, 0)), true, 0, 0, 9, 9, 8, 4, 4, 8);
      return window::on_wm_paint(dc, ps, bHandled);
    }

  private:
    region _region;
    
  };
}

#endif