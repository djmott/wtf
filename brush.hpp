#pragma once
namespace wtf{

  enum class system_colors{
    scrollbar = COLOR_SCROLLBAR,
    background = COLOR_BACKGROUND,
    active_action = COLOR_ACTIVECAPTION,
    inactive_caption = COLOR_INACTIVECAPTION,
    menu = COLOR_MENU,
    window = COLOR_WINDOW,
    window_frame = COLOR_WINDOWFRAME,
    menu_text = COLOR_MENUTEXT,
    window_text = COLOR_WINDOWTEXT,
    caption_text = COLOR_CAPTIONTEXT,
    active_border = COLOR_ACTIVEBORDER,
    inactive_border = COLOR_INACTIVEBORDER,
    app_workspace = COLOR_APPWORKSPACE,
    highlight = COLOR_HIGHLIGHT,
    highlight_text = COLOR_HIGHLIGHTTEXT,
    button_face = COLOR_BTNFACE,
    button_shadow = COLOR_BTNSHADOW,
    gray_text = COLOR_GRAYTEXT,
    button_text = COLOR_BTNTEXT,
    inactive_caption_text = COLOR_INACTIVECAPTIONTEXT,
    button_highlight = COLOR_BTNHIGHLIGHT,
    dark_shadow_3d = COLOR_3DDKSHADOW,
    light_3d = COLOR_3DLIGHT,
    info_text = COLOR_INFOTEXT,
    info_background = COLOR_INFOBK,
    hot_light = COLOR_HOTLIGHT,
    gradient_active_caption = COLOR_GRADIENTACTIVECAPTION,
    gradient_inactive_cation = COLOR_GRADIENTINACTIVECAPTION,
    menu_highlight = COLOR_MENUHILIGHT,
    menu_bar = COLOR_MENUBAR,
  };

  template <system_colors _ID>
  struct system_brush{
    HBRUSH native_handle() const { return GetSysColorBrush(static_cast<int>(_ID)); }
    HBRUSH operator()() const { return GetSysColorBrush(static_cast<int>(_ID)); }
    operator HBRUSH() const { return GetSysColorBrush(static_cast<int>(_ID)); }
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

  //these brushes should be used in the window class during window creation instead of system brushes
  template <system_colors _ID>
  struct create_window_system_brush{
    HBRUSH native_handle() const{ return reinterpret_cast<HBRUSH>(1+ static_cast<int>(_ID)); }
    HBRUSH operator()() const{ return reinterpret_cast<HBRUSH>(1 + static_cast<int>(_ID)); }
    operator HBRUSH() const{ return reinterpret_cast<HBRUSH>(1 + static_cast<int>(_ID)); }
  };

  namespace brushes{

    struct null_brush{
      HBRUSH native_handle() const{ return nullptr; }
      HBRUSH operator()() const{ return nullptr; }
      operator HBRUSH() const{ return nullptr; }
    };

  }

}