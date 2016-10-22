#pragma once
namespace wtf {


    enum class system_colors {
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

    struct rgb {
      COLORREF operator*() const { return _colorref; }

      operator COLORREF() const { return _colorref; }

      uint8_t red() const { return static_cast<uint8_t>(_colorref & 0xff); }

      uint8_t green() const { return static_cast<uint8_t>((_colorref & 0xff00) >> 8); }

      uint8_t blue() const { return static_cast<uint8_t>((_colorref & 0xff0000) >> 16); }

      rgb(uint8_t r, uint8_t g, uint8_t b) : _colorref(RGB(r, g, b)) {}

      explicit rgb(COLORREF newval) : _colorref(newval) {}

      rgb(const rgb &src) : _colorref(src._colorref) {}

      rgb &operator=(const rgb &src) {
        _colorref = src._colorref;
        return *this;
      }

    protected:
      COLORREF _colorref;
    };

    template<system_colors _id>
    struct system_rgb : rgb {
      system_rgb() : rgb(GetSysColor(static_cast<int>(_id))) {}
    };

    template<uint8_t _r, uint8_t _g, uint8_t _b>
    struct static_rgb : rgb {
      static_rgb() : rgb(RGB(_r, _g, _b)) {}
    };

    struct dynamic_rgb : rgb {
      dynamic_rgb(uint8_t _r, uint8_t _g, uint8_t _b) : rgb(RGB(_r, _g, _b)) {}
    };

  }