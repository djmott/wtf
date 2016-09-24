#pragma once
namespace wtf{

  template <UINT _colorID>
  struct system_brush{
    HBRUSH native_handle() const { return reinterpret_cast<HBRUSH>(_colorID); }
    HBRUSH operator()() const { return reinterpret_cast<HBRUSH>(_colorID); }
    operator HBRUSH() const { return reinterpret_cast<HBRUSH>(_colorID); }
  };

  template <COLORREF _Color>
  struct solid_brush{
    solid_brush() : _hbrush(wtf::exception::throw_lasterr_if(CreateSolidBrush(_Color), [](HBRUSH h){ return !h; })){}
    ~solid_brush(){ DeleteObject(_hbrush); }
    HBRUSH native_handle() const { return _hbrush; }
    HBRUSH operator()() const { return _hbrush; }
    operator HBRUSH() const { return _hbrush; }
  protected:
    HBRUSH _hbrush;
  };

  namespace brushes{

    struct null_brush{
      HBRUSH native_handle() const{ return nullptr; }
      HBRUSH operator()() const{ return nullptr; }
      operator HBRUSH() const{ return nullptr; }
    };

    using scrollbar = system_brush< COLOR_SCROLLBAR >;
    using background = system_brush< COLOR_BACKGROUND >;
    using active_action= system_brush< COLOR_ACTIVECAPTION >;
    using inactive_caption = system_brush< COLOR_INACTIVECAPTION >;
    using menu = system_brush< COLOR_MENU >;
    using window = system_brush< COLOR_WINDOW >;
    using window_frame = system_brush< COLOR_WINDOWFRAME >;
    using menu_text = system_brush< COLOR_MENUTEXT >;
    using window_text = system_brush< COLOR_WINDOWTEXT >;
    using caption_text = system_brush< COLOR_CAPTIONTEXT >;
    using active_border = system_brush< COLOR_ACTIVEBORDER >;
    using inactive_border = system_brush< COLOR_INACTIVEBORDER >;
    using app_workspace = system_brush< COLOR_APPWORKSPACE >;
    using highlight = system_brush< COLOR_HIGHLIGHT >;
    using highlight_text= system_brush< COLOR_HIGHLIGHTTEXT >;
    using button_face = system_brush< COLOR_BTNFACE >;
    using button_shadow = system_brush< COLOR_BTNSHADOW >;
    using gray_text = system_brush< COLOR_GRAYTEXT >;
    using button_text = system_brush< COLOR_BTNTEXT >;
    using inactive_caption_text = system_brush< COLOR_INACTIVECAPTIONTEXT >;
    using button_highlight = system_brush< COLOR_BTNHIGHLIGHT >;
    using dark_shadow_3d = system_brush< COLOR_3DDKSHADOW >;
    using light_3d = system_brush< COLOR_3DLIGHT >;
    using info_text = system_brush< COLOR_INFOTEXT >;
    using info_background = system_brush< COLOR_INFOBK >;
    using hot_light = system_brush< COLOR_HOTLIGHT >;
    using gradient_active_caption = system_brush< COLOR_GRADIENTACTIVECAPTION >;
    using gradient_inactive_cation = system_brush< COLOR_GRADIENTINACTIVECAPTION >;
    using menu_highlight = system_brush< COLOR_MENUHILIGHT >;
    using menu_bar = system_brush< COLOR_MENUBAR >;



  }

}