#pragma once

namespace wtf{
  struct device_context : std::unique_ptr<HDC__, std::function<void(HDC)>>{
    using deleter_type = void(*)(HDC);

    ~device_context(){}

    operator HDC() const{ assert(get()); return get(); }

    device_context(device_context&& src) : unique_ptr(std::move(src)){}

    device_context& operator=(device_context&& src){
      unique_ptr::swap(src);
      return *this;
    }

    enum class background_mix_modes{
      opaque = OPAQUE,
      transparent = TRANSPARENT,
    };

    enum class border_edges{
      none = 0,
      thin_raised = BDR_RAISEDOUTER,
      thin_sunken = BDR_SUNKENOUTER,
      bumbed = EDGE_BUMP,
      etched = EDGE_ETCHED,
      raised = EDGE_RAISED,
      sunken = EDGE_SUNKEN,
    };

    enum class border_flags{
      left_border = BF_LEFT,
      top_border = BF_TOP,
      right_border = BF_RIGHT,
      bottom_border = BF_BOTTOM,
      top_left = BF_TOPLEFT,
      top_right = BF_TOPRIGHT,
      bottom_left = BF_BOTTOMLEFT,
      bottom_right = BF_BOTTOMRIGHT,
      whole_rect = BF_RECT,
      diagonal = BF_DIAGONAL,
      diagonal_end_top_right = BF_DIAGONAL_ENDTOPRIGHT,
      diagonal_end_top_left = BF_DIAGONAL_ENDTOPLEFT,
      diagonal_end_bottom_left = BF_DIAGONAL_ENDBOTTOMLEFT,
      diagonal_end_bottom_right = BF_DIAGONAL_ENDBOTTOMRIGHT,
      middle = BF_MIDDLE,
      soft = BF_SOFT,
      adjust = BF_ADJUST,
      flat = BF_FLAT,
      mono = BF_MONO,
    };

    enum class draw_text_flags{
      top = DT_TOP,
      left = DT_LEFT,
      center = DT_CENTER,
      right = DT_RIGHT,
      vcenter = DT_VCENTER,
      bottom = DT_BOTTOM,
      word_break = DT_WORDBREAK,
      single_line = DT_SINGLELINE,
      expand_tabs = DT_EXPANDTABS,
      tab_stop = DT_TABSTOP,
      no_clip = DT_NOCLIP,
      external_leading = DT_EXTERNALLEADING,
      calc_rect = DT_CALCRECT,
      no_prefix = DT_NOPREFIX,
      internal = DT_INTERNAL,
      edit_control = DT_EDITCONTROL,
      path_ellipsis = DT_PATH_ELLIPSIS,
      end_ellipsis = DT_END_ELLIPSIS,
      modify_string = DT_MODIFYSTRING,
      rtl_reading = DT_RTLREADING,
      word_ellipsis = DT_WORD_ELLIPSIS,
      no_full_width_char_break = DT_NOFULLWIDTHCHARBREAK,
      hide_prefix = DT_HIDEPREFIX,
    };

    enum class text_align_modes{
      baseline = TA_BASELINE,
      bottom = TA_BOTTOM,
      top = TA_TOP,
      center = TA_CENTER,
      left = TA_LEFT,
      right = TA_RIGHT,
      no_update_cp = TA_NOUPDATECP,
      rtl_reading = TA_RTLREADING,
      update_cp = TA_UPDATECP,
      vbaseline = VTA_BASELINE,
      vcenter = VTA_CENTER,
    };

    void background_color(const rgb& color) const {
      wtf::exception::throw_lasterr_if(::SetBkColor(*this, color), [](COLORREF c){return CLR_INVALID==c; });
    }
    rgb background_color() const {
      rgb(wtf::exception::throw_lasterr_if(::GetBkColor(*this), [](COLORREF c){return CLR_INVALID==c; }));
    }

    background_mix_modes background_mix_mode() const{
      return static_cast<background_mix_modes>(wtf::exception::throw_lasterr_if(::GetBkMode(*this), [](int i){return !i; }));
    }

    void background_mix_mode(background_mix_modes newval) const{
      wtf::exception::throw_lasterr_if(::SetBkMode(*this, static_cast<int>(newval)), [](int i){return !i; });
    }

    static device_context get_client(HWND hwnd){
      return device_context(wtf::exception::throw_lasterr_if(::GetDC(hwnd), [](HDC dc){ return !dc; }), [hwnd](HDC dc){ ::ReleaseDC(hwnd, dc); });
    }

    static device_context get_window(HWND hwnd){
      return device_context(wtf::exception::throw_lasterr_if(::GetWindowDC(hwnd), [](HDC dc){ return !dc; }), [hwnd](HDC dc){ ::ReleaseDC(hwnd, dc); });
    }

    static device_context get_screen(){
      return device_context(wtf::exception::throw_lasterr_if(::GetDC(nullptr), [](HDC dc){ return !dc; }), [](HDC dc){ ::ReleaseDC(nullptr, dc); });
    }

    void draw_edge(const rect& area, border_edges edge, border_flags flags) const{
      rect r(area);
      wtf::exception::throw_lasterr_if(::DrawEdge(*this, &r, static_cast<UINT>(edge), static_cast<UINT>(flags)), [](BOOL b){return !b; });
    }

    void draw_focus_rect(const rect& area) const{
      wtf::exception::throw_lasterr_if(::DrawFocusRect(*this, &area), [](BOOL b){return !b; });
    }

    void draw_text(const tstring& str, rect& client, draw_text_flags flags) const{
      wtf::exception::throw_lasterr_if(::DrawText(*this, str.c_str(), -1, &client, static_cast<UINT>(flags)), [](BOOL b){return !b; });
    }

    void fill(const region& oRegion, const brush& oBrush) const{
      wtf::exception::throw_lasterr_if(::FillRgn(*this, oRegion, oBrush), [](BOOL b){ return !b; });
    }

    void fill(const rect& oRect, const brush& oBrush) const{
      wtf::exception::throw_lasterr_if(::FillRect(*this, &oRect, oBrush), [](int i){ return !i; });
    }

    void fill(const point::vector& oPoints, const pen& oPen, const brush& oBrush) const{
      select_object(oPen);
      select_object(oBrush);
      wtf::exception::throw_lasterr_if(::Polygon(*this, &oPoints[0], static_cast<int>(oPoints.size())), [](BOOL b){return !b; });
    }

    size get_text_extent(const tstring& str) const{
      size ret;
      wtf::exception::throw_lasterr_if(::GetTextExtentPoint32(*this, str.c_str(), static_cast<int>(str.size()), &ret), [](BOOL b){return !b; });
      return ret;
    }

    void invert(const rect& area) const{
      wtf::exception::throw_lasterr_if(::InvertRect(*this, &area), [](BOOL b){return !b; });
    }

    void line(const pen& oPen, int x1, int y1, int x2, int y2) const{
      select_object(oPen);
      wtf::exception::throw_lasterr_if(::MoveToEx(*this, x1, y1, nullptr), [](BOOL b){ return !b; });
      wtf::exception::throw_lasterr_if(::LineTo(*this, x2, y2), [](BOOL b){ return !b; });
    }

    void move_to(int x, int y) const{
      wtf::exception::throw_lasterr_if(::MoveToEx(*this, x, y, nullptr), [](BOOL b){ return !b; });
    }

    void select_object(const brush& oBrush) const{
      wtf::exception::throw_lasterr_if(::SelectObject(*this, oBrush), [](HGDIOBJ o){ return !o || HGDI_ERROR == o; });
    }

    void select_object(const font::handle& oFont) const{
      wtf::exception::throw_lasterr_if(::SelectObject(*this, oFont), [](HGDIOBJ o){ return !o || HGDI_ERROR == o; });
    }

    void select_object(const pen& oPen) const{
      wtf::exception::throw_lasterr_if(::SelectObject(*this, oPen), [](HGDIOBJ o){ return !o || HGDI_ERROR == o; });
    }

    void select_object(const region& oRegion) const{
      wtf::exception::throw_lasterr_if(::SelectObject(*this, oRegion), [](HGDIOBJ o){ return !o || HGDI_ERROR == o; });
    }

    text_align_modes text_align() const{
      return static_cast<text_align_modes>(wtf::exception::throw_lasterr_if(::GetTextAlign(*this), [](UINT i){ return GDI_ERROR == i; }));
    }
    void text_align(text_align_modes newval) const{
      wtf::exception::throw_lasterr_if(::SetTextAlign(*this, static_cast<UINT>(newval)), [](UINT i){ return GDI_ERROR == i; });
    }

    void text_color(const rgb& color) const {
      wtf::exception::throw_lasterr_if(::SetTextColor(*this, color), [](COLORREF c){ return CLR_INVALID==c; });
    }

    rgb text_color() const {
      return rgb(wtf::exception::throw_lasterr_if(::GetTextColor(*this), [](COLORREF c){ return CLR_INVALID==c; }));
    }

    void text_out(int x, int y, const tstring& str) const{
      wtf::exception::throw_lasterr_if(::TextOut(*this, x, y, str.c_str(), static_cast<int>(str.size())), [](BOOL b){return !b; });
    }

  protected:
    template <typename ... _ArgTs> device_context(_ArgTs&&...oArgs) : unique_ptr(std::forward<_ArgTs>(oArgs)...){}
  };
}
