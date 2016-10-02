#pragma once

namespace wtf{
  namespace default_theme{

    struct panel : wtf::window<panel, has_size, has_border, has_background>{

      virtual const brush& background_brush() const{ return _bkg; }

      panel(HWND parent) : window(parent), _bkg(brush::system_brush(system_colors::hot_light)){}

    protected:
      brush _bkg;
    };

  }
}